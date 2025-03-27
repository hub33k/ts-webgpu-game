interface ICreateBufferOptions {
  label?: string;
}

export function createBuffer(
  data: Float32Array,
  usage: GPUBufferUsageFlags,
  device: GPUDevice,
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
