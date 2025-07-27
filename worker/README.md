# Duck Duck Floor - Cloudflare Worker with Vite

This Cloudflare Worker serves a Vite-powered web application with logging capabilities.

## Features

- **`/` endpoint**: Serves a beautiful "Hello World" Vite app
- **`/log` endpoint**: POST endpoint for logging request data
- **D1 Database**: Ready for data persistence (commented out in code)
- **CORS Support**: Handles cross-origin requests properly

## Setup

1. **Install dependencies**:
   ```bash
   npm install
   ```

2. **Development**:
   ```bash
   # Start the worker in development mode
   npm run dev
   
   # Or start the Vite dev server separately (optional)
   npm run dev:frontend
   ```

3. **Build for production**:
   ```bash
   # Build the frontend assets
   npm run build:frontend
   
   # Deploy to Cloudflare
   npm run deploy
   ```

## Endpoints

### GET `/`
Serves the main Vite application with a beautiful Hello World interface.

### POST `/log`
Accepts JSON data in the request body and logs it to the console.

**Example request**:
```bash
curl -X POST https://your-worker.your-subdomain.workers.dev/log \
  -H "Content-Type: application/json" \
  -d '{"message": "Hello World", "timestamp": "2024-01-01T00:00:00Z"}'
```

**Response**:
```json
{
  "success": true,
  "message": "Log received successfully",
  "timestamp": "2024-01-01T00:00:00.000Z"
}
```

## D1 Database Integration

The worker is configured to use a D1 database (`flow_records`). To enable database logging:

1. Uncomment the database code in `src/index.ts`
2. Create the logs table:
   ```sql
   CREATE TABLE logs (
     id INTEGER PRIMARY KEY AUTOINCREMENT,
     timestamp TEXT NOT NULL,
     data TEXT NOT NULL
   );
   ```

## File Structure

```
worker/
├── src/
│   └── index.ts          # Main worker code
├── public/
│   └── index.html        # Vite app source
├── dist/                 # Built assets (generated)
├── package.json          # Dependencies and scripts
├── vite.config.js        # Vite configuration
├── wrangler.jsonc        # Cloudflare Worker configuration
└── README.md            # This file
```

## Development vs Production

- **Development**: The worker serves inline HTML for quick iteration
- **Production**: After running `npm run build:frontend`, the worker serves optimized static assets from the `dist/` directory

## Testing

Visit your worker URL and click the "Test Log Endpoint" button to test the `/log` endpoint functionality.
