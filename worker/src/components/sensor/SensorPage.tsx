import { Link, useParams } from "react-router";
import { useQuery } from "../../hooks/query/useQuery";
import { ApiResponse, SensorData } from "../../types";
import DataTable from "../DataTable";


const fetchData = async (id: string): Promise<SensorData[] | undefined> => 
    fetch(`/api/data?id=${id}`, { method: 'GET' })
    .then((res: Response): Promise<ApiResponse<SensorData[]>> => res.json())
    .then((json: ApiResponse<SensorData[]>) => {
      const { body } = json;
      return body;
    })


const SensorPage = () => {

    let params = useParams();

    const {data, loading, error, message} = useQuery<SensorData[]>(() => fetchData(params.id!));

    if (loading) {
        return (
            <span>Loading...</span>
        )
    }

    if (error) {
        return (
            <span>{message}</span>
        )
    }

    return (
        <div className="container">
            {/* TODO fix query typing so that data is ensured to be defined here */}
            <h1>Sensor {data![0].sensor_id}</h1>
            <DataTable data={data!}/>
            <Link to="/">Home</Link>
        </div>
    )
}

export default SensorPage;