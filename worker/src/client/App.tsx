import React, { useEffect, useState } from 'react';
import { ApiResponse, SensorData } from '../types';
import DataTable from '../components/DataTable';
import { SensorLocation } from '../components/map/types';
import SSRMap from '../components/map/Maps.lazy';
import { useQuery } from '../hooks/query/useQuery';


const fetchData = async (): Promise<SensorData[] | undefined> => 
    fetch('/api/data', { method: 'GET' })
    .then((res: Response): Promise<ApiResponse<SensorData[]>> => res.json())
    .then((json: ApiResponse<SensorData[]>) => {
      const { body } = json;
      return body;
    })

const App: React.FC = () => {
  const [isLoading, setIsLoading] = useState(false);
  const [resMessage, setMessage] = useState('');

  const testLog = async () => {
    setIsLoading(true);
    setMessage('');
    
    try {
      const response = await fetch('/api/log', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          water_height: 1.1,
          latitude: 40.786,
          longitude: -74.006,
          timestamp: new Date().toISOString(),
          sensor_altitude: 0.5,
          comment: "Test",
          sensor_id: 69
          
        })
      });
      
      if (response.ok) {
        const result: { timestamp: string } = await response.json();
        setMessage(`✅ Log sent successfully! Server responded at ${result.timestamp}`);
      } else {
        setMessage('❌ Failed to send log');
      }
    } catch (error) {
      console.error('Error:', error);
      setMessage('❌ Error sending log');
    } finally {
      setIsLoading(false);
    }
  };


  const {data, loading, error, message} = useQuery<SensorData[]>(fetchData);

  return (
    <div className="container">
      <div className="emoji">🦆</div>
      <h1>Duck Duck Flow</h1>
      <p>Welcome to Duck Duck Flow - a React app with SSR powered by Cloudflare Workers!</p>
      <button 
        className="button" 
        onClick={testLog}
        disabled={isLoading}
      >
        {isLoading ? 'Sending...' : 'Test Log Endpoint'}

      </button>
      <button 
        className="button" 
        onClick={fetchData}
        disabled={isLoading}
      >
        {loading ? 'Fetching...' : 'Fetch Data'}
      </button>
      {data && <DataTable data={data} />}
      {message && (
        <div className="message">
          {message}
        </div>
      )}
      <SSRMap sensors={data?.map((sensorData: SensorData): SensorLocation => {
        return {
          sensor_id: sensorData.sensor_id,
          coordinates: {
            longitude: sensorData.longitude,
            latitude: sensorData.latitude,
          },
        }
      })} />
    </div>
  );
};

export default App;
