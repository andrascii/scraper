import {Job} from "./job";
import { callHandler } from './http';

export class DataModel {
  public static async fetchJobs(): Promise<Job[]> {
    return callHandler<Job[]>('job-list', {
      body: JSON.stringify({ type: 'job-list' }),
      method: 'POST',
    });
  }
}
