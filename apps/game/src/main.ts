// biome-ignore format: off
import './style.css';
import { Engine } from './Engine';
import { Game } from './Game';

const engine = new Engine();

engine.Init().then(async () => {
  const game = new Game(engine);
  await game.Init();

  engine.OnProcessInput = () => {
    game.ProcessInput();
  };
  engine.OnUpdate = (dt: number) => {
    game.Update(dt);
  };
  engine.OnRender = (dt: number) => {
    game.Render(dt);
  };

  engine.Start();
});
