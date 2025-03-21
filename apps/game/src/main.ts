import './style.css';
import { Game } from '~/Game/Game';

let gameModule = await import('./Game/Game');

const state = gameModule.createState();
gameModule.initState(state);

const game = new Game();
await game.Init();

let lastTime = 0;
function frameLoop(time: number) {
  const deltaTime = (time - lastTime) / 1000; // Convert to seconds
  lastTime = time;

  game.Update(deltaTime);
  game.Render();

  requestAnimationFrame(frameLoop);
}

requestAnimationFrame(frameLoop);

// https://vitejs.dev/guide/api-hmr
if (import.meta.hot) {
  // biome-ignore lint/suspicious/noExplicitAny: off
  import.meta.hot.accept('./Game/Game', (module: any) => {
    gameModule = module;
  });
}
