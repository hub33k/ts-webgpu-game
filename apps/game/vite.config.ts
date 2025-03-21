import { type UserConfig, defineConfig } from 'vite';

const PORT = Number(process.env.PORT) || 3000;

// https://vitejs.dev/config/
export default defineConfig({
  server: {
    port: PORT,
    allowedHosts: true,
  },
  resolve: {
    alias: {
      '~': '/src',
    },
  },
  plugins: [],
}) satisfies UserConfig;
