import { World } from './ECS';
import { GameLoop } from './GameLoop';
import type { Vec2 } from './math';
import { Renderer } from './renderer';
import { ResourceManager } from './resources';

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
    // await this.renderer.init();
    this.loop.start();
  }

  // Getters & Setters
  // ================================================================

  public get world(): World {
    return this._world;
  }
}
