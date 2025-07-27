import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';

export default defineConfig({
  plugins: [react()],
  build: {
    ssr: true,
    outDir: 'dist/ssr',
    emptyOutDir: true,
    lib: {
      entry: 'src/client/entry-server.tsx',
      name: 'SSREntry',
      formats: ['es'],
      fileName: 'entry-server'
    },
    rollupOptions: {
      external: ['react', 'react-dom/server']
    }
  }
});
