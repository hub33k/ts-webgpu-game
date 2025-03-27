export * from './geometry';
export * from './renderer.utils';

export class Texture {
  constructor(
    public texture: GPUTexture,
    public sampler: GPUSampler,
  ) {}

  public static async create(device: GPUDevice, image: HTMLImageElement): Promise<Texture> {
    const texture = device.createTexture({
      size: [image.width, image.height, 1],
      format: navigator.gpu.getPreferredCanvasFormat(), // rgba8unorm
      usage:
        GPUTextureUsage.COPY_DST |
        GPUTextureUsage.TEXTURE_BINDING |
        GPUTextureUsage.RENDER_ATTACHMENT,
    });

    const data = await createImageBitmap(image);
    device.queue.copyExternalImageToTexture(
      {
        source: data,
      },
      {
        texture: texture,
      },
      {
        width: image.width,
        height: image.height,
      },
    );

    const sampler = device.createSampler({
      magFilter: 'linear',
      minFilter: 'linear',
    });

    return new Texture(texture, sampler);
  }

  public static async loadTextureFromURL(device: GPUDevice, url: string): Promise<Texture> {
    const image = new Image();
    image.src = url;
    return Texture.create(device, image);
  }
}
