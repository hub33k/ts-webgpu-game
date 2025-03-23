interface IState {
  entities: [];
}

export function createState(): IState {
  return {
    entities: [],
  };
}

export function initState(_state: IState) {}

export function updateState(_deltaTime: number, _state: IState) {}

export function renderState(_state: IState, _pass: GPURenderPassEncoder) {
  // console.log(pass);
}

export class Game {
  public device!: GPUDevice;
  public presentationFormat: GPUTextureFormat = 'bgra8unorm';

  private canvas!: HTMLCanvasElement;
  private context!: GPUCanvasContext;
  private adapter!: GPUAdapter;

  private pipeline!: GPURenderPipeline;

  private vertexBuffer!: GPUBuffer;

  private clearColor = { r: 0.0, g: 0.5, b: 1.0, a: 1.0 };

  public async Init(): Promise<void> {
    await this.InitWebGPU();

    this.CreatePipeline();
  }

  public Update(_deltaTime: number): void {}

  public Render(): void {
    const textureView = this.GetTextureView();
    const renderPassDescriptor: GPURenderPassDescriptor = {
      label: 'Default Render Pass',
      colorAttachments: [
        {
          view: textureView,
          loadOp: 'clear',
          storeOp: 'store',
          clearValue: this.clearColor,
        },
      ],
    };

    const commandEncoder = this.device.createCommandEncoder();
    const pass = commandEncoder.beginRenderPass(renderPassDescriptor);

    pass.setPipeline(this.pipeline);
    pass.setVertexBuffer(0, this.vertexBuffer);
    pass.draw(3);

    pass.end();

    this.device.queue.submit([commandEncoder.finish()]);
  }

  // ================================================================

  public GetTextureView(): GPUTextureView {
    return this.context.getCurrentTexture().createView();
  }

  public ConfigureSurface(): void {
    this.context.configure({
      device: this.device,
      format: this.presentationFormat,
      alphaMode: 'premultiplied', // premultiplied, opaque
    });
  }

  public async InitWebGPU(): Promise<void> {
    if (!navigator.gpu) {
      throw Error('WebGPU not supported.');
    }

    // Request an adapter and device
    const adapter = await navigator.gpu.requestAdapter({
      powerPreference: 'high-performance',
      forceFallbackAdapter: false,
      featureLevel: 'core',
    });
    if (!adapter) {
      throw Error("Couldn't request WebGPU adapter.");
    }
    this.adapter = adapter;

    this.device = await adapter.requestDevice({
      label: 'Default device',
      requiredFeatures: [],
      requiredLimits: {},
    });
    if (!this.device) {
      throw Error("Couldn't request WebGPU device.");
    }

    this.canvas = document.getElementById('canvas') as HTMLCanvasElement;
    if (!this.canvas) {
      throw Error('Canvas not found.');
    }

    // surface
    const context = this.canvas.getContext('webgpu');
    if (!context) {
      throw Error('WebGPU context not found.');
    }
    this.context = context;

    this.presentationFormat = navigator.gpu.getPreferredCanvasFormat();
    this.ConfigureSurface();
  }

  public GetAdapterLimits(): GPUSupportedLimits {
    return this.adapter.limits;
  }

  private CreatePipeline() {
    const shaders = `
struct VertexOut {
  @builtin(position) position : vec4f,
  @location(0) color : vec4f
}

@vertex
fn vertex_main(@location(0) position: vec4f,
               @location(1) color: vec4f) -> VertexOut
{
  var output : VertexOut;
  output.position = position;
  output.color = color;
  return output;
}

@fragment
fn fragment_main(fragData: VertexOut) -> @location(0) vec4f
{
  return fragData.color;
}
`;

    const shaderModule = this.device.createShaderModule({
      code: shaders,
    });

    // biome-ignore format: off
    const vertices = new Float32Array([
      +0.0, +0.6, +0.0, +1.0,  1, 0, 0, 1,
      -0.5, -0.6, +0.0, +1.0,  0, 1, 0, 1,
      +0.5, -0.6, +0.0, +1.0,  0, 0, 1, 1,
    ]);

    this.vertexBuffer = this.device.createBuffer({
      size: vertices.byteLength, // make it big enough to store vertices in
      usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
    });

    this.device.queue.writeBuffer(this.vertexBuffer, 0, vertices, 0, vertices.length);

    const vertexBuffers: GPUVertexBufferLayout[] = [
      {
        attributes: [
          {
            shaderLocation: 0, // position
            offset: 0,
            format: 'float32x4',
          },
          {
            shaderLocation: 1, // color
            offset: 4 * Float32Array.BYTES_PER_ELEMENT, // 4 floats
            format: 'float32x4',
          },
        ],
        arrayStride: 2 * 4 * Float32Array.BYTES_PER_ELEMENT, // 2 * 4 floats
        stepMode: 'vertex',
      },
    ];

    const pipelineDescriptor: GPURenderPipelineDescriptor = {
      label: 'Simple pipeline',
      vertex: {
        module: shaderModule,
        entryPoint: 'vertex_main',
        buffers: vertexBuffers,
      },
      fragment: {
        module: shaderModule,
        entryPoint: 'fragment_main',
        targets: [
          {
            format: navigator.gpu.getPreferredCanvasFormat(),
          },
        ],
      },
      primitive: {
        topology: 'triangle-list',
      },
      layout: 'auto',
    };

    this.pipeline = this.device.createRenderPipeline(pipelineDescriptor);
  }
}
