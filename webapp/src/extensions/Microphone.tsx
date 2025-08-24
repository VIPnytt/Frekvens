import { mdiMicrophone, mdiMicrophoneOff } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from './WebSocket';
import { name as ExtensionsName } from '../services/Extensions';

export const name = 'Microphone';

const [getActive, setActive] = createSignal<boolean>(false);
const [getLevel, setLevel] = createSignal<number>(0);
const [getTreshold, setTreshold] = createSignal<number>(0);

export const MicActive = getActive;

export const receiver = (json: any) => {
    json[name]?.active !== undefined && setActive(json[name].active);
    json[name]?.level !== undefined && setLevel(json[name].level);
    json[name]?.treshold !== undefined && setTreshold(json[name].treshold);
};

const handleActive = () => {
    setActive(!getActive());
    ws.send(JSON.stringify({
        [name]: {
            active: getActive(),
        },
    }));
};

export const Actions: Component = () => (
    <div class="grid grid-cols-[1fr_48px] gap-3 items-center font-medium text-gray-700 hover:text-gray-900">
        <a href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}>
            <Icon
                class="mr-2"
                path={mdiMicrophone}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? 'Deactivate' : 'Activate'} ${name.toLowerCase()}`}>
            <Button
                class={`w-full bg-blue-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'hover:bg-red-600' : 'hover:bg-green-600'}`}
                onClick={handleActive}
            >
                <Icon path={getActive() ? mdiMicrophone : mdiMicrophoneOff} />
            </Button>
        </Tooltip>
    </div>
);

export const Link: Component = () => (
    <Tooltip text="Beat synchronization">
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiMicrophone}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => {
    const handleTreshold = (level: number) => {
        setTreshold(level);
        ws.send(JSON.stringify({
            [name]: {
                treshold: getTreshold(),
            },
        }));
    };

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Sound level</h3>
                    <p class="text-sm text-right text-gray-700 font-semibold mr-4">
                        {getLevel()}
                    </p>
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Treshold</h3>
                    <div class="relative">
                        <input
                            class="text-right w-full py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                            min="1"
                            max={Math.pow(2, 12) - 1}
                            onchange={(e) =>
                                handleTreshold(parseInt(e.currentTarget.value))
                            }
                            step="10"
                            type="number"
                            value={getTreshold()}
                        />
                    </div>
                    <Tooltip text={`${getActive() ? 'Deactivate' : 'Activate'} ${name.toLowerCase()}`}>
                        <Button
                            class={`border-0 px-4 py-3 uppercase leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'bg-red-600' : 'hover:bg-green-600'}`}
                            onClick={handleActive}
                        >
                            <Icon
                                class="mr-2"
                                path={getActive() ? mdiMicrophone : mdiMicrophoneOff}
                            />
                            {getActive() ? 'enabled' : 'disabled'}
                        </Button>
                    </Tooltip>
                </div>
            </div>
        </div>
    );
};

export default MainThird;
