import { useParams } from "react-router";
import { useQuery } from "../../hooks/query/useQuery";
import { ApiResponse, SensorData } from "../../types";


const fetchData = async (id: string): Promise<SensorData | undefined> => 
    fetch(`/api/data?id=${id}`, { method: 'GET' })
    .then((res: Response): Promise<ApiResponse<SensorData>> => res.json())
    .then((json: ApiResponse<SensorData>) => {
      const { body } = json;
      return body;
    });


const SensorPage = () => {

    let params = useParams();

    const {data, loading, error, message} = useQuery<SensorData>(() => fetchData(params.id!));

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
        <div>
            {data?.comment}
        </div>
    )
}

export default SensorPage;