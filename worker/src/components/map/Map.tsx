import { MapContainer, Marker, TileLayer } from "react-leaflet"
import { MapProps } from "./types";

// TODO fix image loading
const Map = (props: MapProps) => {

    const defaultLat = props.coords ? props.coords[0].latitude : 0.00;
    const defaultLon = props.coords ? props.coords[0].longitude: 0.00;

    return (
        <MapContainer center={[defaultLat, defaultLon]} zoom={13} scrollWheelZoom={false} style={{height: "500px"}}>
            <TileLayer
                attribution='&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
                url="https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
            />
            {props.coords && props.coords.map((coord) => <Marker position={[coord.latitude, coord.longitude]}></Marker>)}
        </MapContainer>
    )
}

export default Map;