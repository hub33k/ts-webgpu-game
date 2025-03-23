import { assert } from '~/Game/Utils';

// biome-ignore lint/style/useEnumInitializers: off
export enum ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool,
}

export function ShaderDataTypeSize(type: ShaderDataType): number {
  // biome-ignore format: off
  switch (type) {
    case ShaderDataType.Float: return 4;
    case ShaderDataType.Float2: return 4 * 2;
    case ShaderDataType.Float3: return 4 * 3;
    case ShaderDataType.Float4: return 4 * 4;

    case ShaderDataType.Mat3: return 4 * 3 * 3;
    case ShaderDataType.Mat4: return 4 * 4 * 4;

    case ShaderDataType.Int: return 4;
    case ShaderDataType.Int2: return 4 * 2;
    case ShaderDataType.Int3: return 4 * 3;
    case ShaderDataType.Int4: return 4 * 4;

    case ShaderDataType.Bool: return 1;

    default: assert(false, `Unknown shader data type: ${type}`);
  }
}

// biome-ignore lint/complexity/noStaticOnlyClass: off
export class Shader {
  static CreateModule(device: GPUDevice, source: string): GPUShaderModule {
    const desc: GPUShaderModuleDescriptor = {
      code: source,
    };
    return device.createShaderModule(desc);
  }
}

export class BufferElement {
  public Name!: string;
  public Type!: ShaderDataType;
  public Offset!: number;
  public Size!: number;

  constructor(type: ShaderDataType, name: string) {
    this.Type = type;
    this.Name = name;
    this.Size = ShaderDataTypeSize(type);
    this.Offset = 0;
  }

  public GetComponentCount(): number {
    // biome-ignore format: off
    switch (this.Type) {
      case ShaderDataType.Float: return 1;
      case ShaderDataType.Float2: return 2;
      case ShaderDataType.Float3: return 3;
      case ShaderDataType.Float4: return 4;

      case ShaderDataType.Mat3: return 3; // 3 * float3
      case ShaderDataType.Mat4: return 4; // 4 * float4

      case ShaderDataType.Int: return 1;
      case ShaderDataType.Int2: return 2;
      case ShaderDataType.Int3: return 3;
      case ShaderDataType.Int4: return 3;

      case ShaderDataType.Bool: return 1;

      default: assert(false, `Unknown shader data type: ${this.Type}`);
    }
  }
}

export class BufferLayout {
  private elements: BufferElement[] = [];
  private stride = 0;

  constructor(elements?: BufferElement[]) {
    if (elements) {
      this.elements = elements;
      this.CalculateOffsetsAndStride();
    }
  }

  public GetElements(): BufferElement[] {
    return this.elements;
  }

  public GetStride(): number {
    return this.stride;
  }

  // ================================================================

  private CalculateOffsetsAndStride(): void {
    let offset = 0;
    this.stride = 0;

    for (const element of this.elements) {
      element.Offset = offset;
      offset += element.Size;
      this.stride += element.Size;
    }
  }
}

// biome-ignore lint/complexity/noStaticOnlyClass: off
export class VertexBuffer {
  static Create(
    device: GPUDevice,
    data: Float32Array,
    size: number,
    usage: GPUBufferUsageFlags,
  ): GPUBuffer {
    const buffer = device.createBuffer({
      size,
      usage: usage | GPUBufferUsage.COPY_DST,
    });
    device.queue.writeBuffer(buffer, 0, data);
    return buffer;
  }
}

export class IndexBuffer {}

export function AddVertexBuffer() {}
