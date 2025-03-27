export class QuadGeometry {
  public position!: Float32Array;
  public color!: Float32Array;
  public texCoord!: Float32Array;

  constructor() {
    // biome-ignore format: off
    this.position = new Float32Array([
      // x, y
      -0.5, -0.5, // top left
      +0.5, -0.5, // top right
      -0.5, +0.5, // bottom right

      -0.5, +0.5, // top left
      +0.5, +0.5, // bottom left
      +0.5, -0.5, // bottom right
    ]);

    // biome-ignore format: off
    this.color = new Float32Array([
      // r, g, b
      1.0, 0.0, 1.0,
      1.0, 0.0, 1.0,
      1.0, 0.0, 1.0,

      1.0, 0.0, 1.0,
      1.0, 0.0, 1.0,
      1.0, 0.0, 1.0,
    ]);

    // biome-ignore format: off
    this.texCoord = new Float32Array([
      // u, v
      0.0, 0.0,
      1.0, 0.0,
      0.0, 1.0,

      0.0, 1.0,
      1.0, 1.0,
      1.0, 0.0,
    ]);
  }
}
