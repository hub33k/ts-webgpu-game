import { vec2 } from 'gl-matrix';
import { SECOND_IN_MILLISECONDS } from './constants';
import { InputManager } from './core';

let shaderSource = await import('./shaders/shader.wgsl?raw');

export class Engine {
  // Engine
  public inputManager!: InputManager;
  public gameBounds = vec2.create();

  // Game loop
  private previousTimeInMs = 0;
  private updatePreviousTimeInMs = 0;
  private frameId = 0;
  private lagInMs = 0;
  private updateStepInMs = SECOND_IN_MILLISECONDS / 60;
  private renderFrameTimeInMs = 0;
  private updateFrameTimeInMs = 0;
  private fpsFilterStrength = 20;

  // WebGPU
  private canvas!: HTMLCanvasElement;
  private context!: GPUCanvasContext;
  private device!: GPUDevice;

  private passEncoder!: GPURenderPassEncoder;

  // TMP
  private pipeline!: GPURenderPipeline;
  private vertexBuffer!: GPUBuffer;

  // Callbacks
  public OnUpdate: (dt: number) => void = (_dt) => {};

  public OnRender: (dt: number) => void = (_dt) => {
    this.passEncoder.setVertexBuffer(0, this.vertexBuffer);
    this.passEncoder.setPipeline(this.pipeline);
    this.passEncoder.draw(6);
  };

  public OnProcessInput: () => void = () => {
    if (this.inputManager.wasKeyPressed('Escape')) {
      console.log('ESC');
    }
    if (this.inputManager.wasKeyPressed(' ')) {
      console.log('Space');
    }

    if (this.inputManager.isKeyDown('w')) {
      console.log('w');
    } else if (this.inputManager.isKeyDown('s')) {
      console.log('s');
    }
  };

  public async Init(updateFramesPerSeconds = 60): Promise<void> {
    if (!navigator.gpu) {
      throw Error('WebGPU is not supported. Check https://caniuse.com/webgpu for more info.');
    }

    this.updateStepInMs = SECOND_IN_MILLISECONDS / updateFramesPerSeconds;

    this.canvas = document.getElementById('game') as HTMLCanvasElement;
    if (!this.canvas) {
      throw Error('Canvas not found.');
    }
    this.context = this.canvas.getContext('webgpu') as GPUCanvasContext;
    if (!this.context) {
      throw Error('Failed to get WebGPU context.');
    }

    this.gameBounds[0] = this.canvas.width;
    this.gameBounds[1] = this.canvas.height;

    const adapter = await navigator.gpu.requestAdapter({
      // The powerPreference option is currently ignored when calling requestAdapter() on Windows.
      powerPreference: 'high-performance',
      forceFallbackAdapter: false,
      featureLevel: 'core',
    });
    if (!adapter) {
      throw Error('Failed to get WebGPU adapter.');
    }

    this.device = await adapter.requestDevice({
      label: 'Default device',
      requiredFeatures: [],
      requiredLimits: {},
    });
    if (!this.device) {
      throw Error('Failed to get WebGPU device.');
    }

    this.context.configure({
      device: this.device,
      format: navigator.gpu.getPreferredCanvasFormat(),
      alphaMode: 'premultiplied',
    });

    this.inputManager = new InputManager();

    this.setup_tmp();
  }

  public Start(): void {
    this.previousTimeInMs = performance.now();
    this.frameId = window.requestAnimationFrame(this.Loop);
  }

  public Stop(): void {
    cancelAnimationFrame(this.frameId);
  }

  public Loop: () => void = () => {
    const currentTimeInMs = performance.now();
    const deltaTimeInMs = currentTimeInMs - this.previousTimeInMs;
    this.previousTimeInMs = currentTimeInMs;
    this.lagInMs += Math.min(SECOND_IN_MILLISECONDS, deltaTimeInMs);

    this.OnProcessInput();

    while (this.lagInMs >= this.updateStepInMs) {
      const updateCurrentTimeInMs = performance.now();
      const updateDeltaTimeInMs = updateCurrentTimeInMs - this.updatePreviousTimeInMs;
      this.updateFrameTimeInMs +=
        (updateDeltaTimeInMs - this.updateFrameTimeInMs) / this.fpsFilterStrength;
      this.updatePreviousTimeInMs = updateCurrentTimeInMs;

      this.OnUpdate(updateDeltaTimeInMs / SECOND_IN_MILLISECONDS);
      this.lagInMs -= this.updateStepInMs;
    }

    const commandEncoder = this.device.createCommandEncoder();
    const sceneTextureView = this.context.getCurrentTexture().createView();
    const renderPassDescriptor: GPURenderPassDescriptor = {
      colorAttachments: [
        {
          clearValue: { r: 0.0, g: 0.1, b: 0.1, a: 1.0 },
          loadOp: 'clear',
          storeOp: 'store',
          view: sceneTextureView,
        },
      ],
    };

    this.passEncoder = commandEncoder.beginRenderPass(renderPassDescriptor);

    this.OnRender(this.lagInMs / this.updateStepInMs);

    this.passEncoder.end();
    this.device.queue.submit([commandEncoder.finish()]);

    this.renderFrameTimeInMs += (deltaTimeInMs - this.renderFrameTimeInMs) / this.fpsFilterStrength;
    this.frameId = window.requestAnimationFrame(this.Loop);
  };

  // ================================================================

  // Getters and setters
  // ================================================================

  public GetUpdateFps(): number {
    return SECOND_IN_MILLISECONDS / this.updateFrameTimeInMs;
  }

  public GetRenderFps(): number {
    return SECOND_IN_MILLISECONDS / this.renderFrameTimeInMs;
  }

  // ================================================================

  private async setup_tmp(): Promise<void> {
    this.RecreatePipeline();

    // biome-ignore format: off
    const bufferData = new Float32Array([
      // xy (position)
      -0.5, +0.5, // top left
      +0.5, +0.5, // top right
      +0.5, -0.5, // bottom right

      -0.5, +0.5, // top left
      -0.5, -0.5, // bottom left
      +0.5, -0.5, // bottom right
    ]);
    this.vertexBuffer = this.device.createBuffer({
      label: 'tmp Vertex Buffer',
      size: bufferData.byteLength,
      usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
      // mappedAtCreation: true,
    });
    // new Float32Array(this.vertexBuffer.getMappedRange()).set(bufferData);
    // this.vertexBuffer.unmap();
    this.device.queue.writeBuffer(this.vertexBuffer, 0, bufferData);
  }

  public RecreatePipeline() {
    const shaderModule = this.device.createShaderModule({
      code: shaderSource.default,
    });

    this.pipeline = this.device.createRenderPipeline({
      label: 'TMP Pipeline',
      layout: 'auto',
      vertex: {
        module: shaderModule,
        buffers: [
          {
            arrayStride: 2 * Float32Array.BYTES_PER_ELEMENT,
            attributes: [
              {
                shaderLocation: 0,
                offset: 0,
                format: 'float32x2',
              },
            ],
            stepMode: 'vertex',
          },
        ],
      },
      fragment: {
        module: shaderModule,
        targets: [
          {
            format: navigator.gpu.getPreferredCanvasFormat(),
          },
        ],
      },
      primitive: {
        topology: 'triangle-list',
      },
    });
  }
}

export const engine = new Engine();

if (import.meta.hot) {
  // biome-ignore lint/suspicious/noExplicitAny: off
  import.meta.hot.accept('./shaders/shader.wgsl?raw', async (shader: any) => {
    shaderSource = shader;
    engine.RecreatePipeline();
    console.log(`shader updated at ${Date.now()}`);
  });
}
