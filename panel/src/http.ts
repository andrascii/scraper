export const host = `${window.location.hostname}:${window.location.port}`;

export async function http<T>(request: RequestInfo, requestInit: RequestInit): Promise<T> {
  const response = await fetch(request, requestInit);
  return response.json();
}

export async function callHandler<T>(handler: string, requestInit: RequestInit): Promise<T> {
  // tslint:disable-next-line: no-http-string
  return http<T>(`http://${host}/?${handler}`, requestInit);
}

export async function callHandlerRejectNon2xx<T>(handler: string, requestInit: RequestInit): Promise<T> {
  // tslint:disable-next-line: no-http-string
  return new Promise<T>((resolve: (result: T) => void, reject: (error: Error) => void) => {
    // tslint:disable-next-line: no-http-string
    const url = `http://${host}/?${handler}`;

    const rejectIfNon2xx = (response: Response) => {
      if (response.status !== 200) {
        reject(Error('Unable to update config'));
        return;
      }

      response.json()
        .then((data: T) => resolve(data))
        .catch((error: Error) => reject(error));
    };

    fetch(url, requestInit)
      .then((responseData: Response) => rejectIfNon2xx(responseData))
      .catch((error: Error) => reject(error));
  });
}

export async function callHandlerRejectNon2xxPlainResponse(handler: string, requestInit: RequestInit): Promise<string> {
  // tslint:disable-next-line: no-http-string
  return new Promise<string>((resolve: (result: string) => void, reject: (error: Error) => void) => {
    // tslint:disable-next-line: no-http-string
    const url = `http://${host}/?${handler}`;

    const rejectIfNon2xx = (response: Response) => {
      if (response.status !== 200) {
        reject(Error('Unable to update config'));
        return;
      }

      response.text()
        .then((data: string) => resolve(data))
        .catch((error: Error) => reject(error));
    };

    fetch(url, requestInit)
      .then((responseData: Response) => rejectIfNon2xx(responseData))
      .catch((error: Error) => reject(error));
  });
}
