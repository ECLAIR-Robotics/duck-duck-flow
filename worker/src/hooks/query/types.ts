export type QueryResult<T> = {
    data: T | undefined;
    loading: boolean;
    error: boolean;
    message: string | undefined;
}