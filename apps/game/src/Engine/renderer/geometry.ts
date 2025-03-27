export class QuadGeometry {
  public vertices: number[];
  public indices: number[];

  constructor() {
    // biome-ignore format: off
    this.vertices = [
      // x y       u v        r g b
      -0.5, +0.5,  0.0, 0.0,  1.0, 1.0, 1.0, // 0 - top left
      +0.5, +0.5,  1.0, 0.0,  1.0, 1.0, 1.0, // 1 - top right
      +0.5, -0.5,  1.0, 1.0,  1.0, 1.0, 1.0, // 2 - bottom right
      -0.5, -0.5,  0.0, 1.0,  1.0, 1.0, 1.0, // 3 - bottom left
    ];

    // biome-ignore format: off
    this.indices = [
      0, 1, 2, // First triangle (top left -> top right -> bottom right)
      2, 3, 0, // Second triangle (bottom right -> bottom left -> top left)
    ];
  }
}
