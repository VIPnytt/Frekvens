import { mdiPower, mdiRotate3dVariant, mdiRotateLeftVariant, mdiRotateRightVariant, mdiSleep, mdiTelevision } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Center } from '../components/Center';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { MODEL } from '../config/constants';
import { ws } from '../extensions/WebSocket';
import { SidebarSection } from './WebServer';

export const name = 'Display';

const [getBrightness, setBrightness] = createSignal<number>(0);
const [getColumns, setColumns] = createSignal<number>(0);
const [getOrientation, setOrientation] = createSignal<number>(0);
const [getPixelRatio, setPixelRatio] = createSignal<number>(1);
const [getPower, setPower] = createSignal<boolean>(false);
const [getRatio, setRatio] = createSignal<number>(1);
const [getRows, setRows] = createSignal<number>(0);

export const DisplayBrightness = getBrightness;
export const DisplayColumns = getColumns;
export const DisplayPixelRatio = getPixelRatio;
export const DisplayPower = getPower;
export const DisplayRatio = getRatio;
export const DisplayRows = getRows;

export const receiver = (json: any) => {
    json[name]?.brightness !== undefined && setBrightness(json[name].brightness);
    json[name]?.orientation !== undefined && setOrientation(json[name].orientation);
    json[name]?.pixel?.columns !== undefined && setColumns(json[name].pixel.columns);
    json[name]?.pixel?.ratio !== undefined && setPixelRatio(json[name].pixel?.ratio);
    json[name]?.pixel?.rows !== undefined && setRows(json[name].pixel.rows);
    json[name]?.power !== undefined && setPower(json[name].power);
    json[name]?.ratio !== undefined && setRatio(json[name].ratio);
}

export const Main: Component = () => (
    <Center>
        <h2 class="text-4xl">
            <Icon path={mdiSleep} />
        </h2>
        <p class="text-xl mt-2 text-gray-300">
            Sleeping
        </p>
    </Center>
);

export const Sidebar: Component = () => {
    const handleBrightness = (value: number, send: boolean = true) => {
        setBrightness(value);
        if (send) {
            ws.send(JSON.stringify({
                [name]: {
                    brightness: getBrightness(),
                }
            }));
        }
    };

    const handlePower = () => {
        setPower(!getPower());
        ws.send(JSON.stringify({
            [name]: {
                power: getPower(),
            }
        }));
    };

    return (
        <SidebarSection title={name}>
            <div class="space-y-2">
                <div class="grid grid-cols-[48px_1fr] gap-3 items-center">
                    <Button
                        class={`transition-colors ${getPower() ? 'hover:bg-red-600' : 'bg-red-600 hover:bg-green-600'}`}
                        onClick={handlePower}
                    >
                        <Icon path={mdiPower} />
                    </Button>
                    <Tooltip text={`Brightness ${Math.ceil((getBrightness() + 1) / (Math.pow(2, 8)) * 100)} %`}>
                        <input
                            class="w-full bg-red-600"
                            type="range"
                            min="0"
                            max={Math.pow(2, 8) - 1}
                            value={getBrightness()}
                            onInput={(e) =>
                                handleBrightness(parseFloat(e.currentTarget.value), false)
                            }
                            onKeyUp={(e) =>
                                handleBrightness(getBrightness())
                            }
                            onPointerUp={(e) =>
                                handleBrightness(getBrightness())
                            }
                        />
                    </Tooltip>
                </div>
            </div>
        </SidebarSection>
    );
};

export const SidebarSecondary: Component = () => {
    const handleRotate = (clockwise = true) => {
        setOrientation((getOrientation() + (MODEL || getColumns() === getRows() ? (clockwise ? 90 : 270) : 180)) % 360);
        ws.send(JSON.stringify({
            [name]: {
                orientation: getOrientation(),
            }
        }));
    };

    return (
        <SidebarSection title={name}>
            <div class="space-y-2">
                <div class={`grid ${MODEL || getColumns() === getRows() ? 'grid-cols-[1fr_48px_48px]' : 'grid-cols-[1fr_48px]'} gap-3 items-center font-medium text-gray-700 hover:text-gray-900`}>
                    <div>
                        <Icon
                            class="mr-2"
                            path={mdiTelevision}
                        />
                        Orientation
                    </div>
                    {(MODEL || getColumns() === getRows()) && (
                        <Tooltip text={`${getOrientation()}° to ${(getOrientation() + 270) % 360}°`}>
                            <Button
                                class={`w-full bg-blue-600 hover:bg-blue-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded`}
                                disabled={!getPower()}
                                onClick={() => handleRotate(false)}
                            >
                                <Icon
                                    class={`${getOrientation() === 0 ? '-rotate-90' : getOrientation() === 180 ? 'rotate-90' : getOrientation() === 270 ? 'rotate-180' : ''}`}
                                    path={mdiRotateLeftVariant}
                                />
                            </Button>
                        </Tooltip>
                    )}
                    <Tooltip text={`${getOrientation()}° to ${(getOrientation() + (MODEL || getColumns() === getRows() ? 90 : 180)) % 360}°`}>
                        <Button
                            class="w-full bg-blue-600 hover:bg-blue-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                            disabled={!getPower()}
                            onClick={() => handleRotate()}
                        >
                            <Icon
                                class={`${(MODEL || getColumns() === getRows()) && getOrientation() === 0 ? 'rotate-90' : (MODEL || getColumns() === getRows()) && getOrientation() === 90 ? 'rotate-180' : getOrientation() === 180 ? `${!MODEL && getColumns() !== getRows() ? 'rotate-180' : '-rotate-90'}` : ''}`}
                                path={MODEL || getColumns() === getRows() ? mdiRotateRightVariant : mdiRotate3dVariant}
                            />
                        </Button>
                    </Tooltip>
                </div>
            </div>
        </SidebarSection>
    );
};

export default Main;
