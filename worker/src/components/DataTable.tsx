import { SensorData } from "../types";

type DataTableProps = {
    data: SensorData[];
}

const DataTable = (props: DataTableProps) => {
    return (
        <table>
        <thead>
          <tr>
            <th scope='col'>Sensor Id</th>
            <th scope='col'>Water Height</th>
            <th scope='col'>Timestamp</th>
            <th scope='col'>Comment</th>
          </tr>
        </thead>
        <tbody>
          {props.data.map((datapoint) => {
            return (
            <tr>
              <td scope='row'>{datapoint.sensor_id}</td>
              <td>{datapoint.water_height}</td>
              <td>{datapoint.timestamp}</td>
              <td>{datapoint.comment}</td>
            </tr>
            )
          })}
        </tbody>
      </table>
    );
}

export default DataTable;