import { lazy, Suspense } from "react";
import { MapProps } from "./types";

const LazyMap = lazy(() => import('./Map'));

const LoadableMap = (props: MapProps) => {
    return(
        <Suspense fallback={(<div>Loading...</div>)}>
            <LazyMap coords={props.coords} />
        </Suspense>
    )
}

export default LoadableMap;