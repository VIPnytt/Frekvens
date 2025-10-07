import { createEffect, on } from 'solid-js';

import { ws } from './WebSocket';
import { ConnectivityDomain } from '../services/Connectivity';

export const name = 'WebApp';

export const Canonical = () => {
    const url = `${location.protocol}//${ConnectivityDomain()}`;
    ConnectivityDomain() !== location.hostname && fetch(url, {
        method: 'HEAD',
    }).then((response) => {
        if (response.status === 200) {
            ws.close();
            location.href = url;
        }
    });
}

if (import.meta.env.PROD) {
    createEffect(on(
        ConnectivityDomain,
        Canonical,
        {
            defer: true,
        }
    ));
};

export default Canonical;
