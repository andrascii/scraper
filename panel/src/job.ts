import {Action} from './actions'

export interface Job {
  id: number;
  updateFrequency: number;
  enabled: boolean;
  actions: Action[];
}
