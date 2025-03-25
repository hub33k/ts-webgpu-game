import './style.css';

export class Renderer {
  private canvas!: HTMLCanvasElement;
  private context!: GPUCanvasContext;
  private adapter!: GPUAdapter;
  private device!: GPUDevice;
  private prefreredCanvasFormat!: GPUTextureFormat;

  private clearColor: GPUColor = { r: 0.0, g: 0.5, b: 1.0, a: 1.0 };

  public async Init() {
    if (!navigator.gpu) {
      throw Error('WebGPU is not supported.');
    }

    const canvasElement = document.getElementById('game') as HTMLCanvasElement;
    if (!canvasElement) {
      throw Error('Canvas not found.');
    }
    this.canvas = canvasElement;

    this.context = this.canvas.getContext('webgpu') as GPUCanvasContext;
    if (!this.context) {
      throw Error('Failed to get WebGPU context.');
    }

    this.adapter = (await navigator.gpu.requestAdapter({
      powerPreference: 'high-performance',
      forceFallbackAdapter: false,
      featureLevel: 'core',
    })) as GPUAdapter;
    if (!this.adapter) {
      throw Error('Failed to get WebGPU adapter.');
    }

    this.device = await this.adapter.requestDevice({
      label: 'Default device',
      requiredFeatures: [],
      requiredLimits: {},
    });
    if (!this.device) {
      throw Error('Failed to get WebGPU device.');
    }

    // bgra8unorm
    this.prefreredCanvasFormat = navigator.gpu.getPreferredCanvasFormat();

    this.context.configure({
      device: this.device,
      format: this.prefreredCanvasFormat,
      alphaMode: 'premultiplied',
    });
  }

  public Render() {
    const commandEncoder = this.device.createCommandEncoder();
    const textureView = this.context.getCurrentTexture().createView();
    const renderPassDescriptor: GPURenderPassDescriptor = {
      colorAttachments: [
        {
          view: textureView,
          loadOp: 'clear',
          storeOp: 'store',
          clearValue: this.clearColor,
        },
      ],
    };

    const pass = commandEncoder.beginRenderPass(renderPassDescriptor);

    // render here

    pass.end();
    this.device.queue.submit([commandEncoder.finish()]);
  }
}

const renderer = new Renderer();

renderer.Init().then(() => {
  renderer.Render();
});
