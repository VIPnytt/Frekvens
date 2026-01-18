import { createEventSignal } from '@solid-primitives/event-listener';
import { createReconnectingWS, createWSState } from '@solid-primitives/websocket';
import { batch, createEffect, type ParentComponent } from 'solid-js';

import { HOSTNAME } from '../config/constants';
import { receiver as Connectivity } from '../services/Connectivity';
import { receiver as Device } from '../services/Device';
import { receiver as Display } from '../services/Display';
import { receiver as Extensions } from '../services/Extensions';
import { receiver as Fonts } from '../services/Fonts';
import { receiver as Modes } from '../services/Modes';
import { Canonical as WebAppCanonical } from './WebApp';

export const name = 'WebSocket';
export const WebSocketUrl = `ws://${HOSTNAME}.local/${name.toLowerCase()}`

export const WebSocketWS = createReconnectingWS(`ws://${import.meta.env.PROD ? location.hostname : `${HOSTNAME}.local`}/${name.toLowerCase()}`);
export const WebSocketState = createWSState(WebSocketWS);

createEffect(() => {
    if (WebSocketState() === WebSocketWS.OPEN) {
        WebAppCanonical();
    }
});

export const WebSocketmessages: ParentComponent = (props) => {
    const event = createEventSignal<{ message: MessageEvent }>(WebSocketWS, 'message');
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
