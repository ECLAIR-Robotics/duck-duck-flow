// Additional type definitions for worker bindings
declare global {
  interface Env {
    ASSETS: Fetcher;
    flow_records: D1Database;
  }
}

export {};
