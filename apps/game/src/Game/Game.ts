import type { Engine } from '~/Engine';

export class Game {
  private engine: Engine;

  constructor(engine: Engine) {
    this.engine = engine;
  }

  public ProcessInput(): void {
    if (this.engine.inputManager.wasKeyPressed('Escape')) {
      console.log('ESC');
    }
    if (this.engine.inputManager.wasKeyPressed(' ')) {
      console.log('Space');
    }

    if (this.engine.inputManager.isKeyDown('w')) {
      console.log('w');
    } else if (this.engine.inputManager.isKeyDown('s')) {
      console.log('s');
    }
  }

  public async Init(): Promise<void> {}

  public Update(_dt: number): void {}

  public Render(_dt: number): void {}
}
