export class InputManager {
  private keyDown: { [key: string]: boolean } = {};
  private pressedKeys: { [key: string]: boolean } = {};

  constructor() {
    window.addEventListener('keydown', (e) => {
      if (!this.keyDown[e.key]) {
        this.pressedKeys[e.key] = true; // Mark as newly pressed
      }
      this.keyDown[e.key] = true;
    });

    window.addEventListener('keyup', (e) => {
      this.keyDown[e.key] = false;
      this.pressedKeys[e.key] = false; // Reset when key is released
    });
  }

  public isKeyDown(key: string): boolean {
    return this.keyDown[key];
  }

  public isKeyUp(key: string): boolean {
    return !this.keyDown[key];
  }

  public wasKeyPressed(key: string): boolean {
    if (this.pressedKeys[key]) {
      this.pressedKeys[key] = false; // Reset so it doesn't fire again
      return true;
    }
    return false;
  }
}
