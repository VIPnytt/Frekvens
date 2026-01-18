import { mdiPower, mdiRotate3dVariant, mdiRotateLeftVariant, mdiRotateRightVariant, mdiSleep, mdiTelevision } from '@mdi/js';
import { type Component, createSignal } from 'solid-js';

import { Icon } from '../components/Icon';
import { Tooltip } from '../components/Tooltip';
import { Device } from '../config/devices';
import { SidebarSection } from '../extensions/WebApp';
import { WebSocketWS } from '../extensions/WebSocket';

export const name = 'Display';

const [getBrightness, setBrightness] = createSignal<number>(0);
const [getOrientation, setOrientation] = createSignal<number>(0);
const [getPower, setPower] = createSignal<boolean>(false);

export const DisplayBrightness = getBrightness;
export const DisplayOrientation = getOrientation;
export const DisplayPower = getPower;
export const DisplayPowerSet = setPower;

export const receiver = (json: {
    brightness?: number;
    orientation?: number;
    power?: boolean;
}) => {
    json?.brightness !== undefined && setBrightness(json.brightness);
    json?.orientation !== undefined && setOrientation(json.orientation);
    json?.power !== undefined && setPower(json.power);
};

export const Main: Component = () => (
    <div class="main">
        <div class="text-center">
            <h1>
                <Icon path={mdiSleep} />
            </h1>
            <div class="mode-title">
                Sleeping
            </div>
        </div>
    </div>
);

export const Sidebar: Component = () => {
    const handleBrightness = (value: number, send: boolean = true) => {
        setBrightness(value);
        if (send) {
            WebSocketWS.send(JSON.stringify({
                [name]: {
                    brightness: getBrightness(),
                }
            }));
        }
    };

    const handlePower = () => {
        setPower(!getPower());
        WebSocketWS.send(JSON.stringify({
            [name]: {
                power: getPower(),
            }
        }));
    };

    return (
        <SidebarSection title={name}>
            <div class="space-y-3">
                <div class="grid grid-cols-[--spacing(12)_1fr] gap-3 items-center">
                    <button
                        class={`text-interactive-light dark:text-content-dark ${getPower() ? 'bg-positive-light dark:bg-positive-dark hover:bg-negative-alt-light dark:hover:bg-negative-alt-dark' : 'bg-negative-light dark:bg-negative-dark hover:bg-positive-alt-light dark:hover:bg-positive-alt-dark'}`}
                        onclick={handlePower}
                        type="button"
                    >
                        <Icon path={mdiPower} />
                    </button>
                    <Tooltip text={`Brightness ${Math.ceil(getBrightness() / (2 ** 8 - 1) * 100)} %`}>
                        <input
                            class="w-full"
                            max={2 ** 8 - 1}
                            min="1"
                            onInput={(e) =>
                                handleBrightness(parseFloat(e.currentTarget.value), false)
                            }
                            onKeyUp={() =>
                                handleBrightness(getBrightness())
                            }
                            onPointerUp={() =>
                                handleBrightness(getBrightness())
                            }
                            type="range"
                            value={getBrightness()}
                        />
                    </Tooltip>
                </div>
            </div>
        </SidebarSection>
    );
};

export const SidebarSecondaryComponent: Component = () => {
    const handleRotate = (clockwise = true) => {
        setOrientation((getOrientation() + (Device.GRID_COLUMNS === Device.GRID_ROWS ? (clockwise ? 90 : 270) : 180)) % 360);
        WebSocketWS.send(JSON.stringify({
            [name]: {
                orientation: getOrientation(),
            }
        }));
    };

    return (
        <div class={`action ${Device.GRID_COLUMNS === Device.GRID_ROWS ? 'grid-cols-[1fr_--spacing(12)_--spacing(12)]' : ''}`}>
            <div>
                <Icon
                    class="mr-2"
                    path={mdiTelevision}
                />
                Orientation
            </div>
            {Device.GRID_COLUMNS === Device.GRID_ROWS && (
                <Tooltip text={`${getOrientation()}° to ${(getOrientation() + 270) % 360}°`}>
                    <button
                        class="action-neutral"
                        disabled={!getPower()}
                        onclick={() => handleRotate(false)}
                        type="button"
                    >
                        <Icon
                            class={`${getOrientation() === 0 ? '-rotate-90' : getOrientation() === 180 ? 'rotate-90' : getOrientation() === 270 ? 'rotate-180' : ''}`}
                            path={mdiRotateLeftVariant}
                        />
                    </button>
                </Tooltip>
            )}
            <Tooltip text={`${getOrientation()}° to ${(getOrientation() + (Device.GRID_COLUMNS === Device.GRID_ROWS ? 90 : 180)) % 360}°`}>
                <button
                    class="action-neutral"
                    disabled={!getPower()}
                    onclick={() => handleRotate()}
                    type="button"
                >
                    <Icon
                        class={`${Device.GRID_COLUMNS === Device.GRID_ROWS && getOrientation() === 0 ? 'rotate-90' : Device.GRID_COLUMNS === Device.GRID_ROWS && getOrientation() === 90 ? 'rotate-180' : getOrientation() === 180 ? `${Device.GRID_COLUMNS === Device.GRID_ROWS ? 'rotate-90' : '-rotate-180'}` : ''}`}
                        path={Device.GRID_COLUMNS === Device.GRID_ROWS ? mdiRotateRightVariant : mdiRotate3dVariant}
                    />
                </button>
            </Tooltip>
        </div>
    );
};
