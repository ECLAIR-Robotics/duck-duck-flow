/**
 * Welcome to Cloudflare Workers! This is your first worker.
 *
 * - Run `npm run dev` in your terminal to start a development server
 * - Open a browser tab at http://localhost:8787/ to see your worker in action
 * - Run `npm run deploy` to publish your worker
 *
 * Bind resources to your worker in `wrangler.jsonc`. After adding bindings, a type definition for the
 * `Env` object can be regenerated with `npm run cf-typegen`.
 *
 * Learn more at https://developers.cloudflare.com/workers/
 */


// ! all this is untested code
async function handleRequest(request: any) {
	const upgradeHeader = request.headers.get("Upgrade");	
	if (!upgradeHeader || upgradeHeader.toLowerCase() !== "websocket") {
		return new Response("Expected websocket", { status: 426 });
	}

	const webSocketPair = new WebSocketPair();
	const [client, server] = Object.values(webSocketPair);

	server.accept(); // Accept the server WebSocket and Keep connection alive
	server.addEventListener('message', (event) => {
		console.log("Message from client:", event.data);
		server.send(`Hello from server! You said: ${event.data}`);
	});

	return new Response(null, {
		status: 101,
		webSocket: client,
	});

}

export default {
	async fetch(request, env, ctx): Promise<Response> {
		return handleRequest(request);
	},
} satisfies ExportedHandler<Env>;
