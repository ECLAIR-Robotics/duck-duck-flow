// Additional type definitions for worker bindings
declare global {
  interface Env {
    ASSETS: Fetcher;
    flow_records: D1Database;
  }
}

export type ApiResponse<T> = {
  success: boolean;
  message: string;
  timestamp: string;
  body?: T;
}

export type SensorData = {
  id: number;
  water_height: number;
  latitude: number;
  longitude: number;
  sensor_altitude: number;
  timestamp: string;
  comment: string;
  sensor_id: number;
}

export {};
