import React, { useEffect, useState } from 'react';
import { ApiResponse, SensorData } from '../types';
import DataTable from '../components/DataTable';
import { SensorLocation } from '../components/map/types';
import SSRMap from '../components/map/Maps.lazy';
import duck from '/duck.png';

const App: React.FC = () => {
  const [isLoading, setIsLoading] = useState(false);
  const [message, setMessage] = useState('');
  const [data, setData] = useState<SensorData[] | undefined>();

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
          latitude: 40.756,
          longitude: -74.014,
          timestamp: new Date().toISOString(),
          sensor_altitude: 0.5,
          comment: "Test2",
          sensor_id: 70
          
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

  const fetchData = async () => {
    setIsLoading(true);
    fetch('/api/data', {
      method: 'GET',
    })
    .then((res: Response): Promise<ApiResponse<SensorData[]>> => res.json())
    .then((json: ApiResponse<SensorData[]>) => {
      const { body } = json;
      setData(body);
    }).catch((err) => {
      console.log(`Error fetching data: ${err}`)
      setMessage('❌ Error fetching data')
    })
    setIsLoading(false);
  }

  useEffect(() => {
    fetchData()
  }, [])

  return (
    <div className="container">
      {/* <div className="emoji">🦆</div> */}
      <img src={duck} alt="duck logo" style={{ maxHeight: '20vh' }} />
      <h1>Duck Duck Flow</h1>
      <p className="welcomemsg">Welcome to Duck Duck Flow - a flood sensing system!</p>

      {/* data table */}
      {data && <DataTable data={data} />}
      {message && (
        <div className="message">
          {message}
        </div>
      )}

      {/* fetch more button */}
      <button 
        className="button" 
        onClick={fetchData}
        disabled={isLoading}
      >
        {isLoading ? 'Fetching...' : 'Fetch New Data'}
      </button>

      {/* map */}
      <SSRMap sensors={data?.map((sensorData: SensorData): SensorLocation => {
        return {
          sensor_id: sensorData.sensor_id,
          coordinates: {
            longitude: sensorData.longitude,
            latitude: sensorData.latitude,
          },
        }
      })} />

      <h2>About</h2>
      <h3>Backstory</h3>
      <p>Why did we start? Following the devastating 2025 Texas floods, we learned that sparse sensor 
        coverage in rural and creek bed regions resulted in delayed forecasts and suboptimal emergency 
        mobilization. So we resolved to build the DuckDuckFlow Sensor to close these critical data gaps.</p>
      <h3>Our Mission</h3>
      <p>The DuckDuckFlow Flood Sensor is a wireless, solar-powered, water level monitoring device 
        for deployment in remote flood-prone zones, specifically the Hill Country surrounding Austin. 
        We aim to collect long-term water level data and provide real-time early flood detection to 
        help emergency services mobilize more quickly and effectively, ultimately saving more lives. 
        This project will stay open source, enabling anyone to build the sensor and help gather water 
        level data from remote regions. The project will feature a network of sensors that log data to 
        a centralized database. A separate web app will display this data as a dashboard along with data 
        analysis metrics run asynchronously.</p>
    </div>
  );
};

export default App;
