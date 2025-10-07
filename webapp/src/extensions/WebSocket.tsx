import { batch, createEffect, ParentComponent } from 'solid-js';
import { createEventSignal } from '@solid-primitives/event-listener';
import { createReconnectingWS, createWSState } from '@solid-primitives/websocket';

import { receiver as Connectivity } from '../services/Connectivity';
import { receiver as Device } from '../services/Device';
import { receiver as Display } from '../services/Display';
import { receiver as Extensions } from '../services/Extensions';
import { receiver as Fonts } from '../services/Fonts';
import { receiver as Modes } from '../services/Modes';
import { Canonical as WebAppCanonical } from './WebApp';

export const name = 'WebSocket';

export const ws = createReconnectingWS(`ws://${(!import.meta.env.PROD && import.meta.env.VITE_HOST) || location.hostname}/ws`);
export const WebSocketState = createWSState(ws);

if (import.meta.env.PROD) {
    createEffect(() => {
        if (WebSocketState() === 3) {
            WebAppCanonical;
        }
    });
};

export const MessageProvider: ParentComponent = (props) => {
    const event = createEventSignal<{ message: MessageEvent }>(ws, 'message');

    createEffect(() => {
        const json = JSON.parse(event()?.data || '{}');
        batch(() => {
            Connectivity(json);
            Device(json);
            Display(json);
            Fonts(json);
            Extensions(json);
            Modes(json);
        });
    });
    return (props.children);
};

export default ws;
