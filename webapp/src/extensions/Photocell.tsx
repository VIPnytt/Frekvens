import { mdiBrightness5, mdiBrightnessAuto, mdiSchool } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from './WebSocket';
import { name as ExtensionsName } from '../services/Extensions';

export const name = 'Photocell';

const [getActive, setActive] = createSignal<boolean>(false);
const [getGamma, setGamma] = createSignal<number>(1);
const [getThreshold, setThreshold] = createSignal<number>(0);

export const PhotocellActive = getActive;

export const receiver = (json: any) => {
    json[name]?.active !== undefined && setActive(json[name].active);
    json[name]?.gamma !== undefined && setGamma(json[name].gamma);
    json[name]?.threshold !== undefined && setThreshold(json[name].threshold);
};

const { toast } = Toast();

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
                <Icon path={getActive() ? mdiBrightnessAuto : mdiBrightness5} />
            </Button>
        </Tooltip>
    </div>
);

export const Link: Component = () => (
    <Tooltip text="Automatic ambient brightness adaption">
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

export const MainThird: Component = () => {
    const handleGamma = (gamma: number) => {
        setGamma(gamma);
        ws.send(JSON.stringify({
            [name]: {
                gamma: getGamma(),
            },
        }));
    };

    const handleReset = () => {
        setActive(false);
        ws.send(JSON.stringify({
            [name]: {
                action: 'reset',
            },
        }));
        toast('Learning ambient light...', 10e3);
    };

    const handleThreshold = (threshold: number) => {
        setThreshold(threshold);
        ws.send(JSON.stringify({
            [name]: {
                threshold: getThreshold(),
            },
        }));
    };

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Threshold</h3>
                    <input
                        class="w-full"
                        type="range"
                        max={Math.pow(2, 7) - 1}
                        min="0"
                        value={getThreshold()}
                        onChange={(e) =>
                            handleThreshold(parseInt(e.currentTarget.value))
                        }
                    />
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Brightness</h3>
                    <div class="relative">
                        <span class="absolute left-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                            Gamma:
                        </span>
                        <input
                            class="text-right py-2 my-1 w-full bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200"
                            type="number"
                            max="10"
                            min="0.1"
                            step="0.1"
                            value={getGamma()}
                            onChange={(e) =>
                                handleGamma(parseFloat(e.currentTarget.value))
                            }
                        />
                    </div>
                    <div class="grid grid-cols-[1fr_48px] gap-3 items-center">
                        <Tooltip text={`${getActive() ? 'Deactivate' : 'Activate'} ${name.toLowerCase()}`}>
                            <Button
                                class={`border-0 px-4 py-3 uppercase leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'bg-red-600' : 'hover:bg-green-600'}`}
                                onClick={handleActive}
                            >
                                <Icon
                                    class="mr-2"
                                    path={getActive() ? mdiBrightnessAuto : mdiBrightness5}
                                />
                                {getActive() ? 'enabled' : 'disabled'}
                            </Button>
                        </Tooltip>
                        <Tooltip text="Reset learned brightness range">
                            <Button
                                class="border-0 px-4 py-3 uppercase leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors hover:bg-red-600"
                                disabled={getActive()}
                                onClick={handleReset}
                            >
                                <Icon path={mdiSchool} />
                            </Button>
                        </Tooltip>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default MainThird;
