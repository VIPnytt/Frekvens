import { MODEL as MODEL_FREKVENS } from './ikeaFrekvens';
import { MODEL as MODEL_OBEGRANSAD } from './ikeaObegransad';

const ENV_FREKVENS: boolean = import.meta.env.VITE_ENV_FREKVENS !== undefined;
const ENV_OBEGRANSAD: boolean = import.meta.env.VITE_ENV_OBEGRANSAD !== undefined;

export const EXTENSION_BUILD: boolean = import.meta.env.VITE_EXTENSION_BUILD !== 'false';
export const EXTENSION_HOMEASSISTANT: boolean = import.meta.env.VITE_EXTENSION_HOMEASSISTANT !== 'false';
export const EXTENSION_INFRARED: boolean = import.meta.env.VITE_EXTENSION_INFRARED !== 'false';
export const EXTENSION_MESSAGE: boolean = import.meta.env.VITE_EXTENSION_MESSAGE !== 'false';
export const EXTENSION_MICROPHONE: boolean = import.meta.env.VITE_EXTENSION_MICROPHONE !== 'false';
export const EXTENSION_MQTT: boolean = import.meta.env.VITE_EXTENSION_MQTT !== 'false';
export const EXTENSION_OTA: boolean = import.meta.env.VITE_EXTENSION_OTA !== 'false';
export const EXTENSION_PHOTOCELL: boolean = import.meta.env.VITE_EXTENSION_PHOTOCELL !== 'false';
export const EXTENSION_PLAYLIST: boolean = import.meta.env.VITE_EXTENSION_PLAYLIST !== 'false';
export const EXTENSION_RESTFUL: boolean = import.meta.env.VITE_EXTENSION_REST !== 'false';
export const EXTENSION_SIGNAL: boolean = import.meta.env.VITE_EXTENSION_SIGNAL !== 'false';
export const EXTENSION_WEBAPP: boolean = import.meta.env.VITE_EXTENSION_WEBAPP !== 'false';
export const EXTENSION_WEBSOCKET: boolean = import.meta.env.VITE_EXTENSION_WEBSOCKET !== 'false';

export const MODE_ANIMATION: boolean = import.meta.env.VITE_MODE_ANIMATION !== 'false';
export const MODE_ARTNET: boolean = import.meta.env.VITE_MODE_ARTNET !== 'false';
export const MODE_COUNTDOWN: boolean = import.meta.env.VITE_MODE_COUNTDOWN !== 'false';
export const MODE_DISTRIBUTEDDISPLAYPROTOCOL: boolean = import.meta.env.VITE_MODE_DISTRIBUTEDDISPLAYPROTOCOL !== 'false';
export const MODE_DRAW: boolean = import.meta.env.VITE_MODE_DRAW !== 'false';
export const MODE_E131: boolean = import.meta.env.VITE_MODE_E131 !== 'false';
export const MODE_FLIES: boolean = import.meta.env.VITE_MODE_FLIES !== 'false';
export const MODE_HOMETHERMOMETER: boolean = import.meta.env.VITE_MODE_HOMETHERMOMETER !== 'false';
export const MODE_TICKER: boolean = import.meta.env.VITE_MODE_TICKER !== 'false';

export const MODEL: string | undefined = ENV_FREKVENS ? MODEL_FREKVENS : ENV_OBEGRANSAD ? MODEL_OBEGRANSAD : undefined;
export const NAME: string | undefined = import.meta.env.VITE_NAME;
