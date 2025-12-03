import { mdiRemoteTv, mdiRemoteTvOff } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Icon } from '../components/Icon';
import { Tooltip } from '../components/Tooltip';
import { EXTENSION_MICROPHONE, EXTENSION_PHOTOCELL, EXTENSION_PLAYLIST } from '../config/modules';
import { name as PhotocellName } from './Photocell';
import { name as PlaylistName } from './Playlist';
import { name as MicName } from './Microphone';
import { WebSocketWS } from './WebSocket';
import { name as ExtensionsName } from '../services/Extensions';

export const name = 'Infrared';

const [getActive, setActive] = createSignal<boolean>(false);

export const IrActive = getActive;

export const receiver = (json: any) => {
    json[name]?.active !== undefined && setActive(json[name].active);
};

const handleActive = () => {
    setActive(!getActive());
    WebSocketWS.send(JSON.stringify({
        [name]: {
            active: getActive(),
        },
    }));
};

export const Link: Component = () => (
    <a
        class="link"
        href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
    >
        <Icon
            class="mr-2"
            path={mdiRemoteTv}
        />
        {name}
    </a>
);

export const Actions: Component = () => (
    <div class="action">
        <a href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}>
            <Icon
                class="mr-2"
                path={mdiRemoteTv}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? 'Deactivate' : 'Activate'} ${name.toLowerCase()}`}>
            <button
                class={`w-full ${getActive() ? 'action-activated' : 'action-deactivated'}`}
                onclick={handleActive}
            >
                <Icon path={getActive() ? mdiRemoteTv : mdiRemoteTvOff} />
            </button>
        </Tooltip>
    </div>
);

export const MainThird: Component = () => (
    <div class="main">
        <div class="space-y-3 p-5">
            <h2>
                {name}
            </h2>
            <div class="box">
                <div class="space-y-3">
                    <h3>
                        Remote control
                    </h3>
                    <div class="text-sm">
                        - Power<br />
                        - Brightness<br />
                        - Mode<br />
                        {
                            EXTENSION_MICROPHONE && (
                                <>
                                    - {MicName}<br />
                                </>
                            )
                        }
                        {
                            EXTENSION_PHOTOCELL && (
                                <>
                                    - {PhotocellName}<br />
                                </>
                            )
                        }
                        {
                            EXTENSION_PLAYLIST && (
                                <>
                                    - {PlaylistName}<br />
                                </>
                            )
                        }
                    </div>
                    <button
                        class={`mt-3 w-full ${getActive() ? 'action-activated' : 'action-deactivated'}`}
                        onclick={handleActive}
                    >
                        <Icon
                            class="mr-2"
                            path={getActive() ? mdiRemoteTv : mdiRemoteTvOff}
                        />
                        {getActive() ? 'activated' : 'disabled'}
                    </button>
                </div>
            </div>
        </div>
    </div>
);
