import { createEffect, on } from 'solid-js';

import { ws } from './WebSocket';
import { NetworkDomain } from '../services/Network';

export const name = 'WebApp';

export const Canonical = () => {
    const url = `${location.protocol}//${NetworkDomain()}`;
    NetworkDomain() !== location.hostname && fetch(url, {
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
        NetworkDomain,
        Canonical,
        {
            defer: true,
        }
    ));
};

export default Canonical;
