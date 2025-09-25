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

import './types';
import { render } from './client/entry-server';

	

export default {
	async fetch(request: Request, env: Env, ctx: ExecutionContext): Promise<Response> {
		const url = new URL(request.url);
		const path = url.pathname;

		// Handle POST requests to /log endpoint
		if (path === '/api/log' && request.method === 'POST') {
			try {
				const body = await request.text();
				console.log('Received log data:', body);
				
				// Optional: Store in D1 database if needed
				// You can uncomment and modify this section to store logs in D1
				try {
					await env.flow_records.prepare(
						"INSERT INTO sensor_data (water_height, latitude, longitude, timestamp, sensor_altitude, comment, sensor_id) VALUES (?, ?, ?, ?, ?, ?, ?)"
					).bind(
						JSON.parse(body).water_height,
						JSON.parse(body).latitude,
						JSON.parse(body).longitude,
						JSON.parse(body).timestamp,
						JSON.parse(body).sensor_altitude,
						JSON.parse(body).comment,
						JSON.parse(body).sensor_id
					).run();
				} catch (dbError) {
					console.error('Database error:', dbError);
				}

				return new Response(JSON.stringify({ 
					success: true, 
					message: 'Log received successfully',
					timestamp: new Date().toISOString()
				}), {
					status: 200,
					headers: {
						'Content-Type': 'application/json',
						'Access-Control-Allow-Origin': '*',
						'Access-Control-Allow-Methods': 'POST, OPTIONS',
						'Access-Control-Allow-Headers': 'Content-Type',
					},
				});
			} catch (error) {
				console.error('Error processing log:', error);
				return new Response(JSON.stringify({ 
					success: false, 
					error: 'Failed to process log data' 
				}), {
					status: 500,
					headers: {
						'Content-Type': 'application/json',
						'Access-Control-Allow-Origin': '*',
					},
				});
			}
		}

		// Handle GET request to /data endpoint
		if (path === '/api/data' && request.method === 'GET') {
			try {
				const body = await request.text();
				const dataQueryResult: D1Result | undefined = await env.flow_records.prepare("SELECT * FROM sensor_data ORDER BY datetime(timestamp) DESC LIMIT 10").run();

				return new Response(JSON.stringify({ 
					success: true, 
					message: 'Data fetched successfully',
					// timestamp: new Date().toISOString(),
					body: dataQueryResult?.results,
				}), {
					status: 200,
					headers: {
						'Content-Type': 'application/json',
						'Access-Control-Allow-Origin': '*',
						'Access-Control-Allow-Methods': 'GET, OPTIONS',
						'Access-Control-Allow-Headers': 'Content-Type',
					},
				});
			} catch (error) {
				console.error('Error processing log:', error);
				return new Response(JSON.stringify({ 
					success: false, 
					error: 'Failed to fetch sensor data' 
				}), {
					status: 500,
					headers: {
						'Content-Type': 'application/json',
						'Access-Control-Allow-Origin': '*',
					},
				});
			}
		}

		// Handle CORS preflight requests
		if (request.method === 'OPTIONS') {
			return new Response(null, {
				status: 200,
				headers: {
					'Access-Control-Allow-Origin': '*',
					'Access-Control-Allow-Methods': 'POST, GET, OPTIONS',
					'Access-Control-Allow-Headers': 'Content-Type',
				},
			});
		}

		// Serve the Vite app for all other requests (including '/')
		try {
			// Try to serve from static assets first (production)
			if (env.ASSETS) {
				try {
					const asset = await env.ASSETS.fetch(request);
					if (asset.status !== 404) {
						return asset;
					}
				} catch (assetError) {
					console.log('Asset serving failed, falling back to inline HTML');
				}
			}

			// Fallback to SSR rendered React app
			try {
				const { html } = render();
				const htmlContent = `<!DOCTYPE html>
				<html lang="en">
					<div id="root">${html}</div>
				</html>`;
				return new Response(htmlContent, {
					headers: {
						'Content-Type': 'text/html',
						'Access-Control-Allow-Origin': '*',
					},
				});
			} catch (ssrError) {
				console.error('SSR failed, falling back to basic HTML:', ssrError);
				// Fallback to basic HTML if SSR fails
				const basicHtml = `<!DOCTYPE html>
				<html lang="en">
				Sorry we ran into an error
				</html>`;
				return new Response(basicHtml, {
					headers: {
						'Content-Type': 'text/html',
						'Access-Control-Allow-Origin': '*',
					},
				});
			}
		} catch (error) {
			console.error('Error serving app:', error);
			return new Response('Internal Server Error', { status: 500 });
		}
	},
} satisfies ExportedHandler<Env>;
