import { assert } from '~/Game/Utils';
import { type Matrix3x3, type Vec2, multiply, projection } from '../math';
import type { ResourceManager } from '../resources';
import type { Color } from '../utils';

export type MeshData = {
  id: string;
  vertexData: Float32Array;
  indexData: Uint16Array;
};

type MeshBuffers = {
  vertexBuffer: GPUBuffer;
  indexBuffer: GPUBuffer;
  numIndices: number;
};

type InstanceData = {
  modelMatrix: Matrix3x3;
  color: Color;
};

type TextureEntry = {
  textureView: GPUTextureView;
  sampler: GPUSampler;
  bindGroup: GPUBindGroup;
};

export class Renderer {
  constructor(
    private canvas: HTMLCanvasElement,
    private viewportSize: Vec2,
    private resourceManager: ResourceManager,
  ) {}
}
