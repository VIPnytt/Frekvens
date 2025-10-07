import { mdiImageFrame } from '@mdi/js';
import { Component, createSignal, For, Match, Switch } from 'solid-js';

import { Center } from '../components/Center';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { MODEL, MODE_ANIMATION, MODE_ARTNET, MODE_COUNTDOWN, MODE_DISTRIBUTEDDISPLAYPROTOCOL, MODE_DRAW, MODE_E131, MODE_FLIES, MODE_HOMETHERMOMETER, MODE_TICKER } from '../config/constants';
import { ws } from '../extensions/WebSocket';
import { Main as ModeAnimationMain, Sidebar as ModeAnimationSidebar, receiver as ModeAnimation, name as ModeAnimationName } from '../modes/Animation';
import { Main as ModeArtNetMain, name as ModeArtNetName } from '../modes/ArtNet';
import { Main as ModeCountdownMain, Sidebar as ModeCountdownSidebar, receiver as ModeCountdown, name as ModeCountdownName } from '../modes/Countdown';
import { Main as ModeDistributedDisplayProtocolMain, name as ModeDistributedDisplayProtocolName } from '../modes/DistributedDisplayProtocol';
import { Main as ModeDrawMain, Sidebar as ModeDrawSidebar, receiver as ModeDraw, name as ModeDrawName } from '../modes/Draw';
import { Main as ModeE131Main, name as ModeE131Name } from '../modes/E131';
import { Main as ModeFliesMain, name as ModeFliesName } from '../modes/Flies';
import { Main as ModeHomeThermometerMain, Link as ModeHomeThermometerLink, receiver as ModeHomeThermometer, MainSecondary as ModeHomeThermometerMainThird, name as ModeHomeThermometerName } from '../modes/HomeThermometer';
import { Sidebar as ModeTickerSidebar, receiver as ModeTicker, name as ModeTickerName } from '../modes/Ticker';
import { DeviceModel } from './Device';
import { WebServerPath, SidebarSection } from './WebServer';

export const name = 'Modes';

const [getMode, setMode] = createSignal<string>("");
const [getModes, setModes] = createSignal<string[]>([]);

export const ModesList = getModes;
export const ModesMode = getMode;

export const receiver = (json: any) => {
    json?.[name]?.list !== undefined && setModes(json[name].list);
    json?.[name]?.mode !== undefined && setMode(json[name].mode);
    MODE_ANIMATION && ModeAnimation(json);
    MODE_COUNTDOWN && ModeCountdown(json);
    MODE_DRAW && ModeDraw(json);
    MODE_HOMETHERMOMETER && ModeHomeThermometer(json);
    MODE_TICKER && ModeTicker(json);
};

export const Main: Component = () => (
    <Switch
        fallback={
            <Center>
                <a
                    href={`${REPOSITORY}/wiki/${name}#-${getMode()?.toLowerCase().replace(/\s+/g, '-')}`}
                    target="_blank"
                >
                    <h2 class="text-4xl">{getMode()}</h2>
                </a>
                <p class="text-xs mt-2 text-gray-300">
                    <span class="bold">Tip:</span> There are various <span class="italic">modes</span> to choose from.
                </p>
            </Center>
        }>
        {
            MODE_ANIMATION && (!MODEL || MODEL && DeviceModel() === MODEL) && (
                <Match when={ModesMode() === ModeAnimationName}>
                    <ModeAnimationMain />
                </Match>
            )
        }
        {
            MODE_ARTNET && (
                <Match when={ModesMode() === ModeArtNetName}>
                    <ModeArtNetMain />
                </Match>
            )
        }
        {
            MODE_COUNTDOWN && (
                <Match when={ModesMode() === ModeCountdownName}>
                    <ModeCountdownMain />
                </Match>
            )
        }
        {
            MODE_DISTRIBUTEDDISPLAYPROTOCOL && (
                <Match when={ModesMode() === ModeDistributedDisplayProtocolName}>
                    <ModeDistributedDisplayProtocolMain />
                </Match>
            )
        }
        {
            MODE_DRAW && (!MODEL || MODEL && DeviceModel() === MODEL) && (
                <Match when={ModesMode() === ModeDrawName}>
                    <ModeDrawMain />
                </Match>
            )
        }
        {
            MODE_E131 && (
                <Match when={ModesMode() === ModeE131Name}>
                    <ModeE131Main />
                </Match>
            )
        }
        {
            MODE_FLIES && (
                <Match when={ModesMode() === ModeFliesName}>
                    <ModeFliesMain />
                </Match>
            )
        }
        {
            MODE_HOMETHERMOMETER && (
                <Match when={ModesMode() === ModeHomeThermometerName}>
                    <ModeHomeThermometerMain />
                </Match>
            )
        }
    </Switch>
);

export const Sidebar: Component = () => {
    const handleMode = (mode: string) => {
        setMode(mode)
        ws.send(JSON.stringify({
            [name]: {
                mode: mode,
            },
        }));
    };

    return (
        <>
            <SidebarSection title="Mode">
                <select
                    class="w-full px-2.5 py-2.5 bg-gray-50 border border-gray-200 rounded"
                    value={getMode()}
                    onchange={(e) =>
                        handleMode(e.currentTarget.value)
                    }
                >
                    <For each={getModes()}>
                        {
                            (mode) => <option>{mode}</option>
                        }
                    </For>
                </select>
            </SidebarSection>
            <Switch>
                {
                    MODE_ANIMATION && (!MODEL || MODEL && DeviceModel() === MODEL) && (
                        <Match when={ModesMode() === ModeAnimationName}>
                            <ModeAnimationSidebar />
                        </Match>
                    )
                }
                {
                    MODE_COUNTDOWN && (
                        <Match when={ModesMode() === ModeCountdownName}>
                            <ModeCountdownSidebar />
                        </Match>
                    )
                }
                {
                    MODE_DRAW && (!MODEL || MODEL && DeviceModel() === MODEL) && (
                        <Match when={ModesMode() === ModeDrawName}>
                            <ModeDrawSidebar />
                        </Match>
                    )
                }
                {
                    MODE_TICKER && (
                        <Match when={ModesMode() === ModeTickerName}>
                            <ModeTickerSidebar />
                        </Match>
                    )
                }
            </Switch>
        </>
    );
};

export const SidebarSecondary: Component = () => (
    <SidebarSection title={name}>
        <div class="space-y-2">
            <Tooltip text={`All ${name.toLowerCase()}`}>
                <a
                    href={`#/${name.toLowerCase()}`}
                    class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
                >
                    <Icon
                        class="mr-2"
                        path={mdiImageFrame}
                    />
                    {name} &#40;{ModesList().length}&#41;
                </a>
            </Tooltip>
        </div>
    </SidebarSection>
);

export const MainThird: Component = () => (
    <Switch
        fallback={
            <div class="space-y-3 p-5">
                <h3 class="text-4xl text-white tracking-wide">{name}</h3>
                <div class="bg-white p-6 rounded-md">
                    <div class="space-y-2">
                        <For each={getModes()}>
                            {(mode) => {
                                return (
                                    <p class="text-gray-700 hover:font-semibold">
                                        <a
                                            href={`${REPOSITORY}/wiki/${name}#-${mode.toLowerCase().replace(/\s+/g, '-')}`}
                                            target="_blank"
                                        >
                                            {mode}
                                        </a>
                                    </p>
                                );
                            }}
                        </For>
                    </div>
                </div>
            </div >
        }>
        {
            MODE_HOMETHERMOMETER && (
                <Match when={WebServerPath() === `/${name.toLowerCase()}/${ModeHomeThermometerName.toLowerCase().replace(/\s+/g, '-')}`}>
                    <ModeHomeThermometerMainThird />
                </Match>
            )
        }
    </Switch>
);

export const SidebarThird: Component = () => (
    <SidebarSection title={name}>
        <>
            {
                MODE_HOMETHERMOMETER && (
                    <ModeHomeThermometerLink />
                )
            }
        </>
    </SidebarSection>
);

export default Main;
