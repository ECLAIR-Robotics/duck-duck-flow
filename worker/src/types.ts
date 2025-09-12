// Additional type definitions for worker bindings
declare global {
  interface Env {
    ASSETS: Fetcher;
    flow_records: D1Database;
  }
}

export type ApiResponse<T> = {
  success: boolean;
  message: String;
  timestamp: String;
  body?: T;
}

export type SensorData = {
  id: number;
  water_height: number;
  latitude: number;
  longitude: number;
  sensor_altitude: number;
  timestamp: String;
  comment: String;
  sensor_id: number;
}

export {};
