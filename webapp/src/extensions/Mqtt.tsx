import { mdiInboxFull } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ConnectivityHostname } from '../services/Connectivity';
import { name as ExtensionsName } from '../services/Extensions';

export const name = 'MQTT';

const [getHost, setHost] = createSignal<null | string>(null);

export const receiver = (json: any) => {
    json[name]?.host !== undefined && setHost(json[name].host);
};

export const Link: Component = () => (
    <Tooltip text="Message Queing Telemetry Transport">
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiInboxFull}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => (
    <div class="space-y-3 p-5">
        <h3 class="text-4xl text-white tracking-wide">{name}</h3>
        <div class="bg-white p-6 rounded-md">
            <div class="space-y-2 text-sm text-gray-500">
                {getHost() === null ? (
                    <>
                        <span class="font-medium text-gray-700">
                            Status:
                        </span>
                        &nbsp;
                        Disconnected
                    </>
                ) : (
                    <>
                        <span class="font-medium text-gray-700">
                            Host:
                        </span>
                        &nbsp;
                        <span class="font-mono">
                            {getHost()}
                        </span>
                        <br />
                        <span class="font-medium text-gray-700">
                            Topic:
                        </span>
                        &nbsp;
                        <span class="font-mono">
                            frekvens/{ConnectivityHostname()}
                        </span>
                    </>
                )}
            </div>
        </div>
    </div >
);

export default MainThird;
