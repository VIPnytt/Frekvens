import { mdiDotsGrid, mdiImageFrame } from '@mdi/js';
import { Component, createSignal, For, Match, Switch } from 'solid-js';

import { Icon } from '../components/Icon';
import { MODE_ANIMATION, MODE_ARROW, MODE_ARTNET, MODE_BINARYCLOCK, MODE_BINARYEPOCH, MODE_BLINDS, MODE_BLINK, MODE_BOLDCLOCK, MODE_BREAKOUTCLOCK, MODE_BRIGHT, MODE_CIRCLE, MODE_COUNTDOWN, MODE_DISTRIBUTEDDISPLAYPROTOCOL, MODE_DRAW, MODE_E131, MODE_EQUALIZER, MODE_FIREWORK, MODE_FLIES, MODE_GAMEOFLIFE, MODE_GAMEOFLIFECLOCK, MODE_GLITTER, MODE_GOOGLEWEATHER, MODE_HOMEASSISTANTWEATHER, MODE_HOMETHERMOMETER, MODE_JAGGEDWAVEFORM, MODE_LARGETICKINGCLOCK, MODE_LEAFFALL, MODE_LINES, MODE_METABALLS, MODE_NOISE, MODE_OPENMETEO, MODE_OPENWEATHER, MODE_PINGPONG, MODE_PINGPONGCLOCK, MODE_PIXELSEQUENCE, MODE_RAIN, MODE_RING, MODE_SCAN, MODE_SMALLCLOCK, MODE_SMALLTICKINGCLOCK, MODE_SMOOTHWAVEFORM, MODE_SNAKE, MODE_SNAKECLOCK, MODE_STARS, MODE_TICKER, MODE_WAVEFORM, MODE_WORLDWEATHERONLINE, MODE_WTTRIN, MODE_YR } from '../config/modules';
import { Main as WebAppMain, SidebarSection, WebAppPath, SidebarSectionSecondary } from '../extensions/WebApp';
import { WebSocketWS } from '../extensions/WebSocket';
import { Main as ModeAnimationMain, Sidebar as ModeAnimationSidebar, receiver as ModeAnimation, name as ModeAnimationName } from '../modes/Animation';
import { Main as ModeArrowMain, name as ModeArrowName } from '../modes/Arrow';
import { Main as ModeArtNetMain, name as ModeArtNetName } from '../modes/ArtNet';
import { Main as ModeBinaryClockMain, name as ModeBinaryClockName } from '../modes/BinaryClock';
import { Main as ModeBinaryEpochMain, name as ModeBinaryEpochName } from '../modes/BinaryEpoch';
import { Main as ModeBlindsMain, name as ModeBlindsName } from '../modes/Blinds';
import { Main as ModeBlinkMain, name as ModeBlinkName } from '../modes/Blink';
import { Main as ModeBoldClockMain, name as ModeBoldClockName } from '../modes/BoldClock';
import { Main as ModeBreakoutClockMain, name as ModeBreakoutClockName } from '../modes/BreakoutClock';
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
import { Main as ModeGameOfLifeClockMain, name as ModeGameOfLifeClockName } from '../modes/GameOfLifeClock';
import { Main as ModeGlitterMain, name as ModeGlitterName } from '../modes/Glitter';
import { Main as ModeGoogleWeatherMain, name as ModeGoogleWeatherName } from '../modes/GoogleWeather';
import { Main as ModeHomeAssistantWeatherMain, name as ModeHomeAssistantWeatherName } from '../modes/HomeAssistantWeather';
import { Main as ModeHomeThermometerMain, Link as ModeHomeThermometerLink, receiver as ModeHomeThermometer, MainSecondary as ModeHomeThermometerMainThird, name as ModeHomeThermometerName } from '../modes/HomeThermometer';
import { Main as ModeJaggedWaveformMain, name as ModeJaggedWaveformName } from '../modes/JaggedWaveform';
import { Main as ModeLargeTickingClockMain, Sidebar as ModeLargeTickingClockSidebar, receiver as ModeLargeTickingClock, name as ModeLargeTickingClockName } from '../modes/LargeTickingClock';
import { Main as ModeLeafFallMain, name as ModeLeafFallName } from '../modes/LeafFall';
import { Main as ModeLinesMain, name as ModeLinesName } from '../modes/Lines';
import { Main as ModeMetaballsMain, name as ModeMetaballsName } from '../modes/Metaballs';
import { Main as ModeNoiseMain, name as ModeNoiseName } from '../modes/Noise';
import { Main as ModeOpenMeteoMain, name as ModeOpenMeteoName } from '../modes/OpenMeteo';
import { Main as ModeOpenWeatherMain, name as ModeOpenWeatherName } from '../modes/OpenWeather';
import { Main as ModePingPongMain, name as ModePingPongName } from '../modes/PingPong';
import { Main as ModePingPongClockMain, name as ModePingPongClockName } from '../modes/PingPongClock';
import { Main as ModePixelSequenceMain, name as ModePixelSequenceName } from '../modes/PixelSequence';
import { Main as ModeRainMain, name as ModeRainName } from '../modes/Rain';
import { Main as ModeRingMain, name as ModeRingName } from '../modes/Ring';
import { Main as ModeScanMain, name as ModeScanName } from '../modes/Scan';
import { Main as ModeSmallClockMain, name as ModeSmallClockName } from '../modes/SmallClock';
import { Main as ModeSmallTickingClockMain, Sidebar as ModeSmallTickingClockSidebar, receiver as ModeSmallTickingClock, name as ModeSmallTickingClockName } from '../modes/SmallTickingClock';
import { Main as ModeSmoothWaveformMain, name as ModeSmoothWaveformName } from '../modes/SmoothWaveform';
import { Main as ModeSnakeMain, name as ModeSnakeName } from '../modes/Snake';
import { Main as ModeSnakeClockMain, name as ModeSnakeClockName } from '../modes/SnakeClock';
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
    MODE_LARGETICKINGCLOCK && ModeLargeTickingClock(json);
    MODE_SMALLTICKINGCLOCK && ModeSmallTickingClock(json);
};

export const Main: Component = () => (
    <Switch
        fallback={
            <MainComponent />
        }
    >
        {
            MODE_ANIMATION && (
                <Match when={getMode() === ModeAnimationName}>
                    <ModeAnimationMain />
                </Match>
            )
        }
        {
            MODE_ARROW && (
                <Match when={getMode() === ModeArrowName}>
                    <ModeArrowMain />
                </Match>
            )
        }
        {
            MODE_ARTNET && (
                <Match when={getMode() === ModeArtNetName}>
                    <ModeArtNetMain />
                </Match>
            )
        }
        {
            MODE_BINARYCLOCK && (
                <Match when={getMode() === ModeBinaryClockName}>
                    <ModeBinaryClockMain />
                </Match>
            )
        }
        {
            MODE_BINARYEPOCH && (
                <Match when={getMode() === ModeBinaryEpochName}>
                    <ModeBinaryEpochMain />
                </Match>
            )
        }
        {
            MODE_BLINDS && (
                <Match when={getMode() === ModeBlindsName}>
                    <ModeBlindsMain />
                </Match>
            )
        }
        {
            MODE_BLINK && (
                <Match when={getMode() === ModeBlinkName}>
                    <ModeBlinkMain />
                </Match>
            )
        }
        {
            MODE_BOLDCLOCK && (
                <Match when={getMode() === ModeBoldClockName}>
                    <ModeBoldClockMain />
                </Match>
            )
        }
        {
            MODE_BREAKOUTCLOCK && (
                <Match when={getMode() === ModeBreakoutClockName}>
                    <ModeBreakoutClockMain />
                </Match>
            )
        }
        {
            MODE_BRIGHT && (
                <Match when={getMode() === ModeBrightName}>
                    <ModeBrightMain />
                </Match>
            )
        }
        {
            MODE_CIRCLE && (
                <Match when={getMode() === ModeCircleName}>
                    <ModeCircleMain />
                </Match>
            )
        }
        {
            MODE_COUNTDOWN && (
                <Match when={getMode() === ModeCountdownName}>
                    <ModeCountdownMain />
                </Match>
            )
        }
        {
            MODE_DISTRIBUTEDDISPLAYPROTOCOL && (
                <Match when={getMode() === ModeDistributedDisplayProtocolName}>
                    <ModeDistributedDisplayProtocolMain />
                </Match>
            )
        }
        {
            MODE_DRAW && (
                <Match when={getMode() === ModeDrawName}>
                    <ModeDrawMain />
                </Match>
            )
        }
        {
            MODE_E131 && (
                <Match when={getMode() === ModeE131Name}>
                    <ModeE131Main />
                </Match>
            )
        }
        {
            MODE_EQUALIZER && (
                <Match when={getMode() === ModeEqualizerName}>
                    <ModeEqualizerMain />
                </Match>
            )
        }
        {
            MODE_FIREWORK && (
                <Match when={getMode() === ModeFireworkName}>
                    <ModeFireworkMain />
                </Match>
            )
        }
        {
            MODE_FLIES && (
                <Match when={getMode() === ModeFliesName}>
                    <ModeFliesMain />
                </Match>
            )
        }
        {
            MODE_GAMEOFLIFE && (
                <Match when={getMode() === ModeGameOfLifeName}>
                    <ModeGameOfLifeMain />
                </Match>
            )
        }
        {
            MODE_GAMEOFLIFECLOCK && (
                <Match when={getMode() === ModeGameOfLifeClockName}>
                    <ModeGameOfLifeClockMain />
                </Match>
            )
        }
        {
            MODE_GLITTER && (
                <Match when={getMode() === ModeGlitterName}>
                    <ModeGlitterMain />
                </Match>
            )
        }
        {
            MODE_GOOGLEWEATHER && (
                <Match when={getMode() === ModeGoogleWeatherName}>
                    <ModeGoogleWeatherMain />
                </Match>
            )
        }
        {
            MODE_HOMEASSISTANTWEATHER && (
                <Match when={getMode() === ModeHomeAssistantWeatherName}>
                    <ModeHomeAssistantWeatherMain />
                </Match>
            )
        }
        {
            MODE_HOMETHERMOMETER && (
                <Match when={getMode() === ModeHomeThermometerName}>
                    <ModeHomeThermometerMain />
                </Match>
            )
        }
        {
            MODE_JAGGEDWAVEFORM && (
                <Match when={getMode() === ModeJaggedWaveformName}>
                    <ModeJaggedWaveformMain />
                </Match>
            )
        }
        {
            MODE_LARGETICKINGCLOCK && (
                <Match when={getMode() === ModeLargeTickingClockName}>
                    <ModeLargeTickingClockMain />
                </Match>
            )
        }
        {
            MODE_LEAFFALL && (
                <Match when={getMode() === ModeLeafFallName}>
                    <ModeLeafFallMain />
                </Match>
            )
        }
        {
            MODE_LINES && (
                <Match when={getMode() === ModeLinesName}>
                    <ModeLinesMain />
                </Match>
            )
        }
        {
            MODE_METABALLS && (
                <Match when={getMode() === ModeMetaballsName}>
                    <ModeMetaballsMain />
                </Match>
            )
        }
        {
            MODE_NOISE && (
                <Match when={getMode() === ModeNoiseName}>
                    <ModeNoiseMain />
                </Match>
            )
        }
        {
            MODE_OPENMETEO && (
                <Match when={getMode() === ModeOpenMeteoName}>
                    <ModeOpenMeteoMain />
                </Match>
            )
        }
        {
            MODE_OPENWEATHER && (
                <Match when={getMode() === ModeOpenWeatherName}>
                    <ModeOpenWeatherMain />
                </Match>
            )
        }
        {
            MODE_PINGPONG && (
                <Match when={getMode() === ModePingPongName}>
                    <ModePingPongMain />
                </Match>
            )
        }
        {
            MODE_PINGPONGCLOCK && (
                <Match when={getMode() === ModePingPongClockName}>
                    <ModePingPongClockMain />
                </Match>
            )
        }
        {
            MODE_PIXELSEQUENCE && (
                <Match when={getMode() === ModePixelSequenceName}>
                    <ModePixelSequenceMain />
                </Match>
            )
        }
        {
            MODE_RAIN && (
                <Match when={getMode() === ModeRainName}>
                    <ModeRainMain />
                </Match>
            )
        }
        {
            MODE_RING && (
                <Match when={getMode() === ModeRingName}>
                    <ModeRingMain />
                </Match>
            )
        }
        {
            MODE_SCAN && (
                <Match when={getMode() === ModeScanName}>
                    <ModeScanMain />
                </Match>
            )
        }
        {
            MODE_SMALLCLOCK && (
                <Match when={getMode() === ModeSmallClockName}>
                    <ModeSmallClockMain />
                </Match>
            )
        }
        {
            MODE_SMALLTICKINGCLOCK && (
                <Match when={getMode() === ModeSmallTickingClockName}>
                    <ModeSmallTickingClockMain />
                </Match>
            )
        }
        {
            MODE_SMOOTHWAVEFORM && (
                <Match when={getMode() === ModeSmoothWaveformName}>
                    <ModeSmoothWaveformMain />
                </Match>
            )
        }
        {
            MODE_SNAKE && (
                <Match when={getMode() === ModeSnakeName}>
                    <ModeSnakeMain />
                </Match>
            )
        }
        {
            MODE_SNAKECLOCK && (
                <Match when={getMode() === ModeSnakeClockName}>
                    <ModeSnakeClockMain />
                </Match>
            )
        }
        {
            MODE_STARS && (
                <Match when={getMode() === ModeStarsName}>
                    <ModeStarsMain />
                </Match>
            )
        }
        {
            MODE_TICKER && (
                <Match when={getMode() === ModeTickerName}>
                    <ModeTickerMain />
                </Match>
            )
        }
        {
            MODE_WAVEFORM && (
                <Match when={getMode() === ModeWaveformName}>
                    <ModeWaveformMain />
                </Match>
            )
        }
        {
            MODE_WORLDWEATHERONLINE && (
                <Match when={getMode() === ModeWorldWeatherOnlineName}>
                    <ModeWorldWeatherOnlineMain />
                </Match>
            )
        }
        {
            MODE_WTTRIN && (
                <Match when={getMode() === ModeWttrInName}>
                    <ModeWttrInMain />
                </Match>
            )
        }
        {
            MODE_YR && (
                <Match when={getMode() === ModeYrName}>
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
                        <Match when={getMode() === ModeAnimationName}>
                            <ModeAnimationSidebar />
                        </Match>
                    )
                }
                {
                    MODE_COUNTDOWN && (
                        <Match when={getMode() === ModeCountdownName}>
                            <ModeCountdownSidebar />
                        </Match>
                    )
                }
                {
                    MODE_DRAW && (
                        <Match when={getMode() === ModeDrawName}>
                            <ModeDrawSidebar />
                        </Match>
                    )
                }
                {
                    MODE_LARGETICKINGCLOCK && (
                        <Match when={getMode() === ModeLargeTickingClockName}>
                            <ModeLargeTickingClockSidebar />
                        </Match>
                    )
                }
                {
                    MODE_SMALLTICKINGCLOCK && (
                        <Match when={getMode() === ModeSmallTickingClockName}>
                            <ModeSmallTickingClockSidebar />
                        </Match>
                    )
                }
                {
                    MODE_TICKER && (
                        <Match when={getMode() === ModeTickerName}>
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
