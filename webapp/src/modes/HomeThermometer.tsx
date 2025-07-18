import { mdiHomeThermometer } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Center } from '../components/Center';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { EXTENSION_HOMEASSISTANT, EXTENSION_MQTT, EXTENSION_RESTFUL, EXTENSION_WEBSOCKET } from '../config/constants';
import { name as ExtensionHomeAssistantName } from '../extensions/HomeAssistant';
import { name as ExtensionMqttName } from '../extensions/Mqtt';
import { name as ExtensionRestfulName } from '../extensions/Restful';
import { name as ExtensionWebSocketName } from '../extensions/WebSocket';
import { ExtensionsList } from '../services/Extensions';
import { name as ModesName } from '../services/Modes';
import { NetworkHostname } from '../services/Network';

export const name = 'Home thermometer';

const [getIndoor, setIndoor] = createSignal<number>(21);
const [getOutdoor, setOutdoor] = createSignal<number>(9);

export const receiver = (json: any) => {
    json[name]?.indoor !== undefined && setIndoor(json[name].indoor);
    json[name]?.outdoor !== undefined && setOutdoor(json[name].outdoor);
};

export const Main: Component = () => (
    <Center>
        <h2 class="text-4xl">
            <Icon
                path={mdiHomeThermometer}
            />
        </h2>
        <a href={`#/${ModesName.toLowerCase()}/${name.toLowerCase().replace(/\s+/g, '-')}`}>
            <p class="text-xl mt-2 text-gray-300">
                {name}
            </p>
            <p class="text-sm mt-2 text-gray-300">
                Smart-home integration
            </p>
        </a>
    </Center>
);

export const Link: Component = () => (
    <Tooltip text="Smart-home thermometer">
        <a
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
            href={`#/${ModesName.toLowerCase()}/${name.toLowerCase().replace(/\s+/g, '-')}`}
        >
            <Icon
                class="mr-2"
                path={mdiHomeThermometer}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainSecondary: Component = () => {
    const payload = `{"indoor": ${getIndoor()}, "outdoor": ${getOutdoor()}}`;
    const payloadAlt = `{"${name}": ${payload}}`;

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <p class="text-sm text-gray-700 mb-3">
                        <a
                            href={`${REPOSITORY}/wiki/${ModesName}#-${name.toLowerCase().replace(/\s+/g, '-')}`}
                            target="_blank"
                        >
                            Get started by configuring your <span class="italic">smart-home system</span> to send temperature readings.
                        </a>
                    </p>
                    {
                        EXTENSION_HOMEASSISTANT && ExtensionsList().includes(ExtensionHomeAssistantName) && (
                            <>
                                <div class="border-t border-gray-200" />
                                <h3 class="font-semibold text-gray-700 tracking-wide">
                                    {ExtensionHomeAssistantName}
                                </h3>
                                <p class="text-sm text-gray-500">
                                    <span class="font-medium text-gray-700">Automation:</span> <span class="whitespace-nowrap">Set up via the user-interface.</span>
                                </p>
                            </>
                        )
                    }
                    {
                        EXTENSION_MQTT && ExtensionsList().includes(ExtensionMqttName) && (
                            <>
                                <div class="border-t border-gray-200" />
                                <h3 class="font-semibold text-gray-700 tracking-wide">
                                    {ExtensionMqttName}
                                </h3>
                                <p class="text-sm text-gray-500">
                                    <span class="font-medium text-gray-700">Topic:</span> <span class="font-mono">frekvens/{NetworkHostname()}/{name}/set</span><br />
                                    <span class="font-medium text-gray-700">Message:</span> <span class="font-mono text-gray-500 whitespace-nowrap">{payload}</span>
                                </p>
                            </>
                        )
                    }
                    {
                        EXTENSION_RESTFUL && ExtensionsList().includes(ExtensionRestfulName) && (
                            <>
                                <div class="border-t border-gray-200" />
                                <h3 class="font-semibold text-gray-700 tracking-wide">
                                    {ExtensionRestfulName}
                                </h3>
                                <p class="text-sm text-gray-500">
                                    <span class="font-medium text-gray-700">Method:</span> <span class="font-mono">PATCH</span><br />
                                    <span class="font-medium text-gray-700">URL:</span> <span class="font-mono">{location.protocol}//{location.hostname}/api/{encodeURIComponent(name)}</span><br />
                                    <span class="font-medium text-gray-700">Body:</span> <span class="font-mono text-gray-500 whitespace-nowrap">{payload}</span>
                                </p>
                            </>
                        )
                    }
                    {
                        EXTENSION_WEBSOCKET && ExtensionsList().includes(ExtensionWebSocketName) && (
                            <>
                                <div class="border-t border-gray-200" />
                                <h3 class="font-semibold text-gray-700 tracking-wide">
                                    {ExtensionWebSocketName}
                                </h3>
                                <p class="text-sm text-gray-500">
                                    <span class="font-medium text-gray-700">URL:</span> <span class="font-mono">ws://{location.hostname}/ws</span><br />
                                    <span class="font-medium text-gray-700">Message:</span> <span class="font-mono text-gray-500 whitespace-nowrap">{payloadAlt}</span>
                                </p>
                            </>
                        )
                    }
                </div>
            </div>
        </div >
    );
};

export default Main;
