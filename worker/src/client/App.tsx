import React, { useState } from 'react';

const App: React.FC = () => {
  const [isLoading, setIsLoading] = useState(false);
  const [message, setMessage] = useState('');

  const testLog = async () => {
    setIsLoading(true);
    setMessage('');
    
    try {
      const response = await fetch('/log', {
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

  return (
    <div className="container">
      <div className="emoji">🦆</div>
      <h1>Hello World!</h1>
      <p>Welcome to Duck Duck Floor - a React app with SSR powered by Cloudflare Workers!</p>
      <button 
        className="button" 
        onClick={testLog}
        disabled={isLoading}
      >
        {isLoading ? 'Sending...' : 'Test Log Endpoint'}

      </button>
      {message && (
        <div className="message">
          {message}
        </div>
      )}
    </div>
  );
};

export default App;
