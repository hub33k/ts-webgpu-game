import { vec2 } from 'gl-matrix';
import { SECOND_IN_MILLISECONDS } from './constants';
import { InputManager } from './core';

export class Engine {
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

  // Engine
  public inputManager!: InputManager;
  public gameBounds = vec2.create();

  // Callbacks
  public OnUpdate: (dt: number) => void = (_dt) => {};

  public OnRender: (dt: number) => void = (_dt) => {};

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
      throw Error('WebGPU is not supported.');
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

    this.device = await adapter.requestDevice();
    if (!this.device) {
      throw Error('Failed to get WebGPU device.');
    }

    this.context.configure({
      device: this.device,
      format: navigator.gpu.getPreferredCanvasFormat(),
      alphaMode: 'premultiplied',
    });

    this.inputManager = new InputManager();
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

  public GetUpdateFps(): number {
    return SECOND_IN_MILLISECONDS / this.updateFrameTimeInMs;
  }

  public GetRenderFps(): number {
    return SECOND_IN_MILLISECONDS / this.renderFrameTimeInMs;
  }
}
