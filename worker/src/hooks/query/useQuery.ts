import { useCallback, useEffect, useState } from "react"
import { QueryResult } from "./types"

export const useQuery = <T>(queryFunction: () => Promise<T | undefined>) => {
    const [queryResult, setQueryResult] = useState<QueryResult<T>>({
        data: undefined,
        loading: true,
        error: false,
        message: undefined,
    })

    const cachedFn = useCallback(() => queryFunction(), [])

    useEffect(() => {
        cachedFn().then((data) => {
            if (data === undefined) {
                setQueryResult({
                    data: undefined,
                    loading: false,
                    error: true,
                    message: 'Data undefined' // TODO get the actual error message here
                });
            } else {
                setQueryResult({
                    data,
                    loading: false,
                    error: false,
                    message: undefined,
                });
            }
        }).catch((error: Error) => {
            setQueryResult({
                data: undefined,
                loading: false,
                error: true,
                message: error.message // TODO get the actual error message here
            });
        });
    }, [cachedFn]);

    return queryResult;
}