import { mdiDotsGrid, mdiImageFrame } from '@mdi/js';
import { Component, createSignal, For, Match, Switch } from 'solid-js';

import { MainComponent as ClockMainComponent } from '../components/Clock';
import { Icon } from '../components/Icon';
import { MODE_ANIMATION, MODE_ARROW, MODE_ARTNET, MODE_BINARYEPOCH, MODE_BLINDS, MODE_BLINK, MODE_BRIGHT, MODE_CIRCLE, MODE_COUNTDOWN, MODE_DISTRIBUTEDDISPLAYPROTOCOL, MODE_DRAW, MODE_E131, MODE_EQUALIZER, MODE_FIREWORK, MODE_FLIES, MODE_GAMEOFLIFE, MODE_GLITTER, MODE_GOOGLEWEATHER, MODE_HOMEASSISTANTWEATHER, MODE_HOMETHERMOMETER, MODE_JAGGEDWAVEFORM, MODE_LEAFFALL, MODE_LINES, MODE_METABALLS, MODE_NOISE, MODE_OPENMETEO, MODE_OPENWEATHER, MODE_PINGPONG, MODE_PIXELSEQUENCE, MODE_RAIN, MODE_RING, MODE_SCAN, MODE_SMOOTHWAVEFORM, MODE_SNAKE, MODE_STARS, MODE_TICKER, MODE_WAVEFORM, MODE_WORLDWEATHERONLINE, MODE_WTTRIN, MODE_YR } from '../config/modules';
import { Main as WebAppMain, SidebarSection, WebAppPath, SidebarSectionSecondary } from '../extensions/WebApp';
import { WebSocketWS } from '../extensions/WebSocket';
import { Main as ModeAnimationMain, Sidebar as ModeAnimationSidebar, receiver as ModeAnimation, name as ModeAnimationName } from '../modes/Animation';
import { Main as ModeArrowMain, name as ModeArrowName } from '../modes/Arrow';
import { Main as ModeArtNetMain, name as ModeArtNetName } from '../modes/ArtNet';
import { Main as ModeBinaryEpochMain, name as ModeBinaryEpochName } from '../modes/BinaryEpoch';
import { Main as ModeBlindsMain, name as ModeBlindsName } from '../modes/Blinds';
import { Main as ModeBlinkMain, name as ModeBlinkName } from '../modes/Blink';
import { Main as ModeBrightMain, name as ModeBrightName } from '../modes/Bright';
import { Main as ModeCircleMain, name as ModeCircleName } from '../modes/Circle';
import { Main as ModeCountdownMain, Sidebar as ModeCountdownSidebar, Actions as ModeCountdownActions, Link as ModeCountdownLink, receiver as ModeCountdown, name as ModeCountdownName } from '../modes/Countdown';
import { Main as ModeDistributedDisplayProtocolMain, name as ModeDistributedDisplayProtocolName } from '../modes/DistributedDisplayProtocol';
import { Main as ModeDrawMain, Sidebar as ModeDrawSidebar, receiver as ModeDraw, name as ModeDrawName } from '../modes/Draw';
import { Main as ModeE131Main, name as ModeE131Name } from '../modes/E131';
import { Main as ModeEqualizerMain, name as ModeEqualizerName } from '../modes/Equalizer';
import { Main as ModeFireworkMain, name as ModeFireworkName } from '../modes/Firework';
import { Main as ModeFliesMain, name as ModeFliesName } from '../modes/Flies';
import { Main as ModeGameOfLifeMain, name as ModeGameOfLifeName } from '../modes/GameOfLife';
import { Main as ModeGlitterMain, name as ModeGlitterName } from '../modes/Glitter';
import { Main as ModeGoogleWeatherMain, name as ModeGoogleWeatherName } from '../modes/GoogleWeather';
import { Main as ModeHomeAssistantWeatherMain, name as ModeHomeAssistantWeatherName } from '../modes/HomeAssistantWeather';
import { Main as ModeHomeThermometerMain, Link as ModeHomeThermometerLink, receiver as ModeHomeThermometer, MainSecondary as ModeHomeThermometerMainThird, name as ModeHomeThermometerName } from '../modes/HomeThermometer';
import { Main as ModeJaggedWaveformMain, name as ModeJaggedWaveformName } from '../modes/JaggedWaveform';
import { Main as ModeLeafFallMain, name as ModeLeafFallName } from '../modes/LeafFall';
import { Main as ModeLinesMain, name as ModeLinesName } from '../modes/Lines';
import { Main as ModeMetaballsMain, name as ModeMetaballsName } from '../modes/Metaballs';
import { Main as ModeNoiseMain, name as ModeNoiseName } from '../modes/Noise';
import { Main as ModeOpenMeteoMain, name as ModeOpenMeteoName } from '../modes/OpenMeteo';
import { Main as ModeOpenWeatherMain, name as ModeOpenWeatherName } from '../modes/OpenWeather';
import { Main as ModePingPongMain, name as ModePingPongName } from '../modes/PingPong';
import { Main as ModePixelSequenceMain, name as ModePixelSequenceName } from '../modes/PixelSequence';
import { Main as ModeRainMain, name as ModeRainName } from '../modes/Rain';
import { Main as ModeRingMain, name as ModeRingName } from '../modes/Ring';
import { Main as ModeScanMain, name as ModeScanName } from '../modes/Scan';
import { Main as ModeSmoothWaveformMain, name as ModeSmoothWaveformName } from '../modes/SmoothWaveform';
import { Main as ModeSnakeMain, name as ModeSnakeName } from '../modes/Snake';
import { Main as ModeStarsMain, name as ModeStarsName } from '../modes/Stars';
import { Main as ModeTickerMain, Sidebar as ModeTickerSidebar, receiver as ModeTicker, name as ModeTickerName } from '../modes/Ticker';
import { Main as ModeWaveformMain, name as ModeWaveformName } from '../modes/Waveform';
import { Main as ModeWorldWeatherOnlineMain, name as ModeWorldWeatherOnlineName } from '../modes/WorldWeatherOnline';
import { Main as ModeWttrInMain, name as ModeWttrInName } from '../modes/WttrIn';
import { Main as ModeYrMain, name as ModeYrName } from '../modes/Yr';
import { DisplayPowerSet } from './Display';

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
            getMode().toLowerCase().includes("clock") ? <ClockMainComponent /> : <MainComponent />
        }
    >
        {
            MODE_ANIMATION && (
                <Match when={ModesMode() === ModeAnimationName}>
                    <ModeAnimationMain />
                </Match>
            )
        }
        {
            MODE_ARROW && (
                <Match when={ModesMode() === ModeArrowName}>
                    <ModeArrowMain />
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
            MODE_BINARYEPOCH && (
                <Match when={ModesMode() === ModeBinaryEpochName}>
                    <ModeBinaryEpochMain />
                </Match>
            )
        }
        {
            MODE_BLINDS && (
                <Match when={ModesMode() === ModeBlindsName}>
                    <ModeBlindsMain />
                </Match>
            )
        }
        {
            MODE_BLINK && (
                <Match when={ModesMode() === ModeBlinkName}>
                    <ModeBlinkMain />
                </Match>
            )
        }
        {
            MODE_BRIGHT && (
                <Match when={ModesMode() === ModeBrightName}>
                    <ModeBrightMain />
                </Match>
            )
        }
        {
            MODE_CIRCLE && (
                <Match when={ModesMode() === ModeCircleName}>
                    <ModeCircleMain />
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
            MODE_DRAW && (
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
            MODE_EQUALIZER && (
                <Match when={ModesMode() === ModeEqualizerName}>
                    <ModeEqualizerMain />
                </Match>
            )
        }
        {
            MODE_FIREWORK && (
                <Match when={ModesMode() === ModeFireworkName}>
                    <ModeFireworkMain />
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
            MODE_GAMEOFLIFE && (
                <Match when={ModesMode() === ModeGameOfLifeName}>
                    <ModeGameOfLifeMain />
                </Match>
            )
        }
        {
            MODE_GLITTER && (
                <Match when={ModesMode() === ModeGlitterName}>
                    <ModeGlitterMain />
                </Match>
            )
        }
        {
            MODE_GOOGLEWEATHER && (
                <Match when={ModesMode() === ModeGoogleWeatherName}>
                    <ModeGoogleWeatherMain />
                </Match>
            )
        }
        {
            MODE_HOMEASSISTANTWEATHER && (
                <Match when={ModesMode() === ModeHomeAssistantWeatherName}>
                    <ModeHomeAssistantWeatherMain />
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
        {
            MODE_JAGGEDWAVEFORM && (
                <Match when={ModesMode() === ModeJaggedWaveformName}>
                    <ModeJaggedWaveformMain />
                </Match>
            )
        }
        {
            MODE_LEAFFALL && (
                <Match when={ModesMode() === ModeLeafFallName}>
                    <ModeLeafFallMain />
                </Match>
            )
        }
        {
            MODE_LINES && (
                <Match when={ModesMode() === ModeLinesName}>
                    <ModeLinesMain />
                </Match>
            )
        }
        {
            MODE_METABALLS && (
                <Match when={ModesMode() === ModeMetaballsName}>
                    <ModeMetaballsMain />
                </Match>
            )
        }
        {
            MODE_NOISE && (
                <Match when={ModesMode() === ModeNoiseName}>
                    <ModeNoiseMain />
                </Match>
            )
        }
        {
            MODE_OPENMETEO && (
                <Match when={ModesMode() === ModeOpenMeteoName}>
                    <ModeOpenMeteoMain />
                </Match>
            )
        }
        {
            MODE_OPENWEATHER && (
                <Match when={ModesMode() === ModeOpenWeatherName}>
                    <ModeOpenWeatherMain />
                </Match>
            )
        }
        {
            MODE_PINGPONG && (
                <Match when={ModesMode() === ModePingPongName}>
                    <ModePingPongMain />
                </Match>
            )
        }
        {
            MODE_PIXELSEQUENCE && (
                <Match when={ModesMode() === ModePixelSequenceName}>
                    <ModePixelSequenceMain />
                </Match>
            )
        }
        {
            MODE_RAIN && (
                <Match when={ModesMode() === ModeRainName}>
                    <ModeRainMain />
                </Match>
            )
        }
        {
            MODE_RING && (
                <Match when={ModesMode() === ModeRingName}>
                    <ModeRingMain />
                </Match>
            )
        }
        {
            MODE_SCAN && (
                <Match when={ModesMode() === ModeScanName}>
                    <ModeScanMain />
                </Match>
            )
        }
        {
            MODE_SMOOTHWAVEFORM && (
                <Match when={ModesMode() === ModeSmoothWaveformName}>
                    <ModeSmoothWaveformMain />
                </Match>
            )
        }
        {
            MODE_SNAKE && (
                <Match when={ModesMode() === ModeSnakeName}>
                    <ModeSnakeMain />
                </Match>
            )
        }
        {
            MODE_STARS && (
                <Match when={ModesMode() === ModeStarsName}>
                    <ModeStarsMain />
                </Match>
            )
        }
        {
            MODE_TICKER && (
                <Match when={ModesMode() === ModeTickerName}>
                    <ModeTickerMain />
                </Match>
            )
        }
        {
            MODE_WAVEFORM && (
                <Match when={ModesMode() === ModeWaveformName}>
                    <ModeWaveformMain />
                </Match>
            )
        }
        {
            MODE_WORLDWEATHERONLINE && (
                <Match when={ModesMode() === ModeWorldWeatherOnlineName}>
                    <ModeWorldWeatherOnlineMain />
                </Match>
            )
        }
        {
            MODE_WTTRIN && (
                <Match when={ModesMode() === ModeWttrInName}>
                    <ModeWttrInMain />
                </Match>
            )
        }
        {
            MODE_YR && (
                <Match when={ModesMode() === ModeYrName}>
                    <ModeYrMain />
                </Match>
            )
        }
    </Switch>
);

export const Sidebar: Component = () => {
    const handleMode = (mode: string) => {
        setMode(mode)
        WebSocketWS.send(JSON.stringify({
            [name]: {
                mode: mode,
            },
        }));
        DisplayPowerSet(true);
    };

    return (
        <>
            <SidebarSection title="Mode">
                <select
                    class="w-full"
                    name={name}
                    onchange={(e) =>
                        handleMode(e.currentTarget.value)
                    }
                    value={getMode()}
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
                    MODE_ANIMATION && (
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
                    MODE_DRAW && (
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
    <SidebarSectionSecondary title={name}>
        <div class="space-y-3">
            {
                MODE_COUNTDOWN && (
                    <ModeCountdownActions />
                )
            }
            <a
                class="link"
                href={`#/${name.toLowerCase()}`}
            >
                <Icon
                    class="mr-2"
                    path={mdiImageFrame}
                />
                More {name.toLowerCase()}...
            </a>
        </div>
    </SidebarSectionSecondary>
);

export const MainThird: Component = () => (
    <Switch
        fallback={
            <WebAppMain />
        }
    >
        {
            MODE_COUNTDOWN && (
                <Match when={WebAppPath() === `/${name.toLowerCase()}/${ModeCountdownName.toLowerCase()}`}>
                    <ModeCountdownMain />
                </Match>
            )
        }
        {
            MODE_HOMETHERMOMETER && (
                <Match when={WebAppPath() === `/${name.toLowerCase()}/${ModeHomeThermometerName.toLowerCase().replace(/\s+/g, '-')}`}>
                    <ModeHomeThermometerMainThird />
                </Match>
            )
        }
    </Switch>
);

export const SidebarThird: Component = () => (
    <SidebarSectionSecondary title={name}>
        <>
            {
                MODE_COUNTDOWN && (
                    <ModeCountdownLink />
                )
            }
            {
                MODE_HOMETHERMOMETER && (
                    <ModeHomeThermometerLink />
                )
            }
        </>
    </SidebarSectionSecondary>
);

export const MainComponent: Component<{
    icon?: string;
    internal?: boolean;
    text?: string;
}> = (props) => (
    <div class="main">
        <h1>
            <Icon path={props.icon || mdiDotsGrid} />
        </h1>
        <a
            class="mode-title"
            href={`${props.internal ? `#/${name.toLowerCase()}/` : `https://github.com/VIPnytt/Frekvens/wiki/${name}#-`}${getMode().toLowerCase().replace(/\s+/g, '-')}`}
            target={props.internal ? '_self' : '_blank'}
        >
            {getMode()}
        </a>
        {
            props.text && (
                <div class="mode-subtitle">
                    {props.text}
                </div>
            )
        }
    </div>
);
