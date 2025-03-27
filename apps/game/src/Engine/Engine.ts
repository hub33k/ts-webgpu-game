import { vec2 } from 'gl-matrix';
import { SECOND_IN_MILLISECONDS } from './constants';
import { InputManager } from './core';
import { BufferUtils, QuadGeometry, Texture } from './renderer';

let shaderSource = await import('./shaders/shader.wgsl?raw');
let uvTestImage = await import('/assets/tmp/uv_test.png');

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
  private indexBuffer!: GPUBuffer;
  private testTexture!: Texture;
  private textureBindGroup!: GPUBindGroup;

  // Callbacks
  public OnUpdate: (dt: number) => void = (_dt) => {};

  public OnRender: (dt: number) => void = (_dt) => {
    this.passEncoder.setPipeline(this.pipeline);
    this.passEncoder.setBindGroup(0, this.textureBindGroup);
    this.passEncoder.setVertexBuffer(0, this.vertexBuffer);
    this.passEncoder.setIndexBuffer(this.indexBuffer, 'uint16');
    this.passEncoder.drawIndexed(6, 1, 0, 0, 0);
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

    if (this.inputManager.isKeyDown('a')) {
      console.log('a');
    } else if (this.inputManager.isKeyDown('d')) {
      console.log('d');
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

    await this.setup_tmp();
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
    await this.LoadImages();

    const geometry = new QuadGeometry();

    // vertex buffer
    {
      const bufferData = new Float32Array(geometry.vertices);
      this.vertexBuffer = BufferUtils.create(this.device, bufferData, GPUBufferUsage.VERTEX, {
        label: 'vertex buffer',
      });
    }

    // index buffer
    {
      const bufferData = new Uint16Array(geometry.indices);
      this.indexBuffer = BufferUtils.create(this.device, bufferData, GPUBufferUsage.INDEX, {
        label: 'index buffer',
      });
    }

    this.RecreatePipeline();
  }

  public RecreatePipeline() {
    const shaderModule = this.device.createShaderModule({
      code: shaderSource.default,
    });

    const textureBindGroupLayout: GPUBindGroupLayout = this.device.createBindGroupLayout({
      entries: [
        {
          binding: 0,
          visibility: GPUShaderStage.FRAGMENT,
          sampler: {},
        },
        {
          binding: 1,
          visibility: GPUShaderStage.FRAGMENT,
          texture: {},
        },
      ],
    });

    const pipelineLayout: GPUPipelineLayout = this.device.createPipelineLayout({
      bindGroupLayouts: [textureBindGroupLayout],
    });

    this.textureBindGroup = this.device.createBindGroup({
      layout: textureBindGroupLayout,
      entries: [
        {
          binding: 0,
          resource: this.testTexture.sampler,
        },
        {
          binding: 1,
          resource: this.testTexture.texture.createView(),
        },
      ],
    });

    this.pipeline = this.device.createRenderPipeline({
      label: 'TMP Pipeline',
      layout: pipelineLayout,
      vertex: {
        module: shaderModule,
        buffers: [
          {
            arrayStride: 7 * Float32Array.BYTES_PER_ELEMENT,
            attributes: [
              // position - x, y
              {
                shaderLocation: 0,
                offset: 0,
                format: 'float32x2',
              },
              // texture coords - u, v
              {
                shaderLocation: 1,
                offset: 2 * Float32Array.BYTES_PER_ELEMENT,
                format: 'float32x2',
              },
              // color - r, g, b
              {
                shaderLocation: 2,
                offset: 4 * Float32Array.BYTES_PER_ELEMENT,
                format: 'float32x3',
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
            blend: {
              // S - source (image), T - target (background), SA - source alpha
              // S * 1 + (T * (1 - SA))
              color: {
                srcFactor: 'one',
                dstFactor: 'one-minus-src-alpha',
                operation: 'add',
              },
              alpha: {
                srcFactor: 'one',
                dstFactor: 'one-minus-src-alpha',
                operation: 'add',
              },
            },
          },
        ],
      },
      primitive: {
        topology: 'triangle-list',
      },
    });
  }

  public async LoadImages(reload = false) {
    this.testTexture = await Texture.loadTextureFromModule(this.device, uvTestImage);

    if (reload) {
      // Replace only the bind group to use the new texture
      this.textureBindGroup = this.device.createBindGroup({
        layout: this.pipeline.getBindGroupLayout(0),
        entries: [
          { binding: 0, resource: this.testTexture.sampler },
          { binding: 1, resource: this.testTexture.texture.createView() },
        ],
      });
    }
  }
}

export const engine = new Engine();

// https://bjornlu.com/blog/hot-module-replacement-is-easy
if (import.meta.hot) {
  // biome-ignore lint/suspicious/noExplicitAny: off
  import.meta.hot.accept('./shaders/shader.wgsl?raw', async (shader: any) => {
    shaderSource = shader;
    engine.RecreatePipeline();
    console.info(`shader updated at ${Date.now()}`);
  });

  import.meta.hot.accept(
    '/assets/tmp/uv_test.png',
    // biome-ignore lint/suspicious/noExplicitAny: off
    async (module: any) => {
      uvTestImage = module;
      await engine.LoadImages(true);
      console.info(`uv_test.png updated at ${Date.now()}`);
    },
  );
}
