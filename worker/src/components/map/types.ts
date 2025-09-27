export type MultiMapProps = {
    sensors?: SensorLocation[];
}

export type SensorLocation = {
  sensor_id: number;
  coordinates: Coordinates
}

export type Coordinates = {
  longitude: number;
  latitude: number;
}