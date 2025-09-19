import { mdiBrightnessAuto, mdiBrightnessPercent } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from './WebSocket';
import { name as ExtensionsName } from '../services/Extensions';

export const name = 'Photocell';

const [getActive, setActive] = createSignal<boolean>(false);

export const PhotocellActive = getActive;

export const receiver = (json: any) => {
    json[name]?.active !== undefined && setActive(json[name].active);
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
                path={mdiBrightnessAuto}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? 'Deactivate' : 'Activate'} ${name.toLowerCase()}`}>
            <Button
                class={`w-full bg-blue-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'hover:bg-red-600' : 'hover:bg-green-600'}`}
                onClick={handleActive}
            >
                <Icon path={getActive() ? mdiBrightnessAuto : mdiBrightnessPercent} />
            </Button>
        </Tooltip>
    </div>
);

export const Link: Component = () => (
    <Tooltip text="Ambient brightness adaption">
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiBrightnessAuto}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => (
    <div class="space-y-3 p-5">
        <h3 class="text-4xl text-white tracking-wide">{name}</h3>
        <div class="bg-white p-6 rounded-md">
            <div class="space-y-2">
                <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                    Ambient brightness adaption
                </h3>
                <p class="text-sm">
                    Automatically dims the display for comfortable viewing.
                </p>
                <Tooltip text={`${getActive() ? 'Deactivate' : 'Activate'} ${name.toLowerCase()}`}>
                    <Button
                        class={`border-0 px-4 py-3 uppercase leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'bg-red-600' : 'hover:bg-green-600'}`}
                        onClick={handleActive}
                    >
                        <Icon
                            class="mr-2"
                            path={getActive() ? mdiBrightnessAuto : mdiBrightnessPercent}
                        />
                        {getActive() ? 'enabled' : 'disabled'}
                    </Button>
                </Tooltip>
            </div>
        </div>
    </div>
);

export default MainThird;
