import { lazy, Suspense } from "react";
import { MultiMapProps } from "./types";

const LazyMap = lazy(() => import('./MultiMap'));

const SSRMap = (props: MultiMapProps) => {
    return(
        <Suspense fallback={(<div>Loading...</div>)}>
            <LazyMap sensors={props.sensors} />
        </Suspense>
    )
}

export default SSRMap;