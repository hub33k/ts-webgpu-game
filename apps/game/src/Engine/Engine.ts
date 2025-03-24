import { World } from './ECS';
import { GameLoop } from './GameLoop';
import type { Vec2 } from './math';
import { Renderer } from './renderer';
import {
  type Resource,
  type ResourceLoader,
  ResourceManager,
  type ResourceType,
} from './resources';

export class Engine {
  private renderer: Renderer;
  private loop: GameLoop;
  private _world: World;
  private resourceManager: ResourceManager = new ResourceManager();

  constructor(canvas: HTMLCanvasElement, viewportSize: Vec2) {
    this.renderer = new Renderer(canvas, viewportSize, this.resourceManager);
    this._world = new World();
    this.loop = new GameLoop(60, this._world, this.renderer);
  }

  public async Init() {
    await this.renderer.init();
    this.loop.start();
  }

  public addResourceLoader<T extends Resource>(
    resourceType: ResourceType,
    loader: ResourceLoader<T>,
  ) {
    this.resourceManager.addLoader(resourceType, loader);
  }

  public async load(url: string, resourceType: ResourceType) {
    this.resourceManager.load(url, resourceType);
  }

  // Getters & Setters
  // ================================================================

  public get world(): World {
    return this._world;
  }

  public getRenderFps(): number {
    return this.loop.getRenderFps();
  }

  public getUpdateFps(): number {
    return this.loop.getUpdateFps();
  }

  public getGpuTime() {
    return this.renderer.gpuTime;
  }
}
