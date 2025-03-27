interface ICreateBufferOptions {
  label?: string;
}

export class BufferUtils {
  public static create(
    device: GPUDevice,
    data: Float32Array | Uint16Array,
    usage: GPUBufferUsageFlags,
    options: ICreateBufferOptions = {},
  ): GPUBuffer {
    const buffer = device.createBuffer({
      size: data.byteLength,
      usage: GPUBufferUsage.COPY_DST | usage,
    });

    if (options.label) {
      buffer.label = options.label;
    }

    device.queue.writeBuffer(buffer, 0, data);

    return buffer;
  }
}
