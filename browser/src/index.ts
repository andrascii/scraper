import {waitFor} from './wait_for';

//async function fetchJob(): Promise<boolean> {
//  return false;
//}

(async () => {
  while(true) {
    console.log('iteration');
    await waitFor(10000);
  }
})();