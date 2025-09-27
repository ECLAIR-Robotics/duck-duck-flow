import { MapContainer, Marker, Popup, TileLayer } from "react-leaflet"
import { MultiMapProps, SensorLocation } from "./types";

// Displays Multiple Markers
const MultiMap = (props: MultiMapProps) => {

    const defaultLat = props.sensors ? props.sensors[0].coordinates.latitude : 0.00;
    const defaultLon = props.sensors ? props.sensors[0].coordinates.longitude: 0.00;

    return (
        <MapContainer center={[defaultLat, defaultLon]} zoom={13} scrollWheelZoom={false} style={{height: "500px"}}>
            <TileLayer
                attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
                url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
            />
            {props.sensors && props.sensors.map((sensor: SensorLocation) => 
                <Marker position={[sensor.coordinates.latitude, sensor.coordinates.longitude]}>
                    <Popup>
                        ID: {sensor.sensor_id} <br/> Lat: {sensor.coordinates.latitude} <br/> Lon: {sensor.coordinates.longitude}
                    </Popup>
                </Marker>
            )}
        </MapContainer>
    )
}

export default MultiMap;