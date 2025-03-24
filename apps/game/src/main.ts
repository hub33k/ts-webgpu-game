import './style.css';
import { Engine } from '~/Engine';
import { Vec2 } from '~/Engine/math';

// Get the canvas element from the DOM
const canvas = document.getElementById('canvas') as HTMLCanvasElement;
if (!canvas) {
  throw Error('Canvas not found.');
}
const viewportSize = new Vec2(1152, 648);
const engine = new Engine(canvas, viewportSize);

const world = engine.world;

engine.Init();

// if (import.meta.hot) {
//   import.meta.hot.accept();
// }

// ================================================================

// import { Game } from '~/Game/Game';

// let gameModule = await import('./Game/Game');

// const state = gameModule.createState();
// gameModule.initState(state);

// const game = new Game();
// await game.Init();

// let lastTime = 0;
// function frameLoop(time: number) {
//   const deltaTime = (time - lastTime) / 1000; // Convert to seconds
//   lastTime = time;

//   game.Update(deltaTime);
//   game.Render();

//   requestAnimationFrame(frameLoop);
// }

// requestAnimationFrame(frameLoop);

// // https://vitejs.dev/guide/api-hmr
// if (import.meta.hot) {
//   // biome-ignore lint/suspicious/noExplicitAny: off
//   import.meta.hot.accept('./Game/Game', (module: any) => {
//     gameModule = module;
//   });
// }
