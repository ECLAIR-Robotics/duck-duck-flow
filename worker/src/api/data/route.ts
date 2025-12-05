export const fulfillDataRequest = (request: Request, env: Env) => {
    const searchParams = (new URL(request.url)).searchParams;
    if (searchParams.size === 0) {
        return getData(request, env);
    } else {
        const id = searchParams.get("id");
        console.log(id);
        if (id !== null) {
            return getDataById(request, env, id);
        }
        // TODO handle no id
    }
    return new Response(JSON.stringify({ 
        success: false, 
        error: 'Failed to fetch sensor data' 
    }), {
        status: 500,
        headers: {
            'Content-Type': 'application/json',
            'Access-Control-Allow-Origin': '*',
        },
    });
}

const getData = async (request: Request, env: Env) => {
    try {
        const dataQueryResult: D1Result | undefined = await env.flow_records.prepare("SELECT * FROM sensor_data ORDER BY datetime(timestamp) DESC LIMIT 10").run();

        return new Response(JSON.stringify({ 
            success: true, 
            message: 'Data fetched successfully',
            // timestamp: new Date().toISOString(),
            body: dataQueryResult?.results,
        }), {
            status: 200,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Methods': 'GET, OPTIONS',
                'Access-Control-Allow-Headers': 'Content-Type',
            },
        });
    } catch (error) {
        console.error('Error processing log:', error);
        return new Response(JSON.stringify({ 
            success: false, 
            error: 'Failed to fetch sensor data' 
        }), {
            status: 500,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
            },
        });
    }
}

const getDataById = async (request: Request, env: Env, id: string) => {
    try {
        const dataQueryResult: D1Result | undefined = await env.flow_records.prepare(`SELECT * FROM sensor_data WHERE sensor_id = ${id} ORDER BY datetime(timestamp) DESC LIMIT 1`).run();

        return new Response(JSON.stringify({ 
            success: true, 
            message: 'Data fetched successfully',
            body: dataQueryResult?.results[0],
        }), {
            status: 200,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Methods': 'GET, OPTIONS',
                'Access-Control-Allow-Headers': 'Content-Type',
            },
        });
    } catch (error) {
        console.error('Error processing log:', error);
        return new Response(JSON.stringify({ 
            success: false, 
            error: 'Failed to fetch sensor data' 
        }), {
            status: 500,
            headers: {
                'Content-Type': 'application/json',
                'Access-Control-Allow-Origin': '*',
            },
        });
    }
}