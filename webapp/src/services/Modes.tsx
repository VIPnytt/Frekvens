import { mdiDotsGrid, mdiImageFrame } from "@mdi/js";
import { type Component, createSignal, For, Match, Switch } from "solid-js";

import { Icon } from "../components/Icon";
import {
    MODE_ANIMATION,
    MODE_ARROW,
    MODE_BINARYCLOCK,
    MODE_BINARYEPOCH,
    MODE_BLINDS,
    MODE_BLINK,
    MODE_BREAKOUTCLOCK,
    MODE_BRIGHT,
    MODE_CIRCLE,
    MODE_COUNTDOWN,
    MODE_DRAW,
    MODE_EQUALIZER,
    MODE_FIREWORK,
    MODE_FLIES,
    MODE_GAMEOFLIFE,
    MODE_GLITTER,
    MODE_GOOGLEWEATHER,
    MODE_HOMEASSISTANTWEATHER,
    MODE_HOMETHERMOMETER,
    MODE_JAGGEDWAVEFORM,
    MODE_LARGECLOCK,
    MODE_LEAFFALL,
    MODE_LINES,
    MODE_METABALLS,
    MODE_NOISE,
    MODE_OPENMETEO,
    MODE_OPENWEATHER,
    MODE_PINGPONG,
    MODE_PINGPONGCLOCK,
    MODE_PIXELSEQUENCE,
    MODE_RAIN,
    MODE_RING,
    MODE_SCAN,
    MODE_SMALLCLOCK,
    MODE_SMOOTHWAVEFORM,
    MODE_SNAKE,
    MODE_STARS,
    MODE_STREAM,
    MODE_TICKER,
    MODE_WAVEFORM,
    MODE_WORLDWEATHERONLINE,
    MODE_WTTRIN,
    MODE_YR,
} from "../config/modules";
import { SidebarSection, SidebarSectionSecondary, Main as WebAppMain, WebAppPath } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { Main as ModeAnimationMain, name as ModeAnimationName, Sidebar as ModeAnimationSidebar } from "../modes/Animation";
import { Main as ModeArrowMain, name as ModeArrowName } from "../modes/Arrow";
import { Main as ModeBinaryClockMain, name as ModeBinaryClockName } from "../modes/BinaryClock";
import { Main as ModeBinaryEpochMain, name as ModeBinaryEpochName } from "../modes/BinaryEpoch";
import { Main as ModeBlindsMain, name as ModeBlindsName } from "../modes/Blinds";
import { Main as ModeBlinkMain, name as ModeBlinkName } from "../modes/Blink";
import { Main as ModeBreakoutClockMain, name as ModeBreakoutClockName } from "../modes/BreakoutClock";
import { Main as ModeBrightMain, name as ModeBrightName } from "../modes/Bright";
import { Main as ModeCircleMain, name as ModeCircleName } from "../modes/Circle";
import { Actions as ModeCountdownActions, Link as ModeCountdownLink, Main as ModeCountdownMain, name as ModeCountdownName, Sidebar as ModeCountdownSidebar } from "../modes/Countdown";
import { Main as ModeDrawMain, name as ModeDrawName, Sidebar as ModeDrawSidebar } from "../modes/Draw";
import { Main as ModeEqualizerMain, name as ModeEqualizerName } from "../modes/Equalizer";
import { Main as ModeFireworkMain, name as ModeFireworkName } from "../modes/Firework";
import { Main as ModeFliesMain, name as ModeFliesName } from "../modes/Flies";
import { Main as ModeGameOfLifeMain, name as ModeGameOfLifeName, Sidebar as ModeGameOfLifeSidebar } from "../modes/GameOfLife";
import { Main as ModeGlitterMain, name as ModeGlitterName } from "../modes/Glitter";
import { Main as ModeGoogleWeatherMain, name as ModeGoogleWeatherName } from "../modes/GoogleWeather";
import { Main as ModeHomeAssistantWeatherMain, name as ModeHomeAssistantWeatherName } from "../modes/HomeAssistantWeather";
import { Link as ModeHomeThermometerLink, Main as ModeHomeThermometerMain, MainSecondary as ModeHomeThermometerMainThird, name as ModeHomeThermometerName } from "../modes/HomeThermometer";
import { Main as ModeJaggedWaveformMain, name as ModeJaggedWaveformName } from "../modes/JaggedWaveform";
import { Main as ModeLargeClockMain, name as ModeLargeClockName, Sidebar as ModeLargeClockSidebar } from "../modes/LargeClock";
import { Main as ModeLeafFallMain, name as ModeLeafFallName } from "../modes/LeafFall";
import { Main as ModeLinesMain, name as ModeLinesName } from "../modes/Lines";
import { Main as ModeMetaballsMain, name as ModeMetaballsName } from "../modes/Metaballs";
import { Main as ModeNoiseMain, name as ModeNoiseName } from "../modes/Noise";
import { Main as ModeOpenMeteoMain, name as ModeOpenMeteoName } from "../modes/OpenMeteo";
import { Main as ModeOpenWeatherMain, name as ModeOpenWeatherName } from "../modes/OpenWeather";
import { Main as ModePingPongMain, name as ModePingPongName } from "../modes/PingPong";
import { Main as ModePingPongClockMain, name as ModePingPongClockName } from "../modes/PingPongClock";
import { Main as ModePixelSequenceMain, name as ModePixelSequenceName } from "../modes/PixelSequence";
import { Main as ModeRainMain, name as ModeRainName } from "../modes/Rain";
import { Main as ModeRingMain, name as ModeRingName } from "../modes/Ring";
import { Main as ModeScanMain, name as ModeScanName } from "../modes/Scan";
import { Main as ModeSmallClockMain, name as ModeSmallClockName, Sidebar as ModeSmallClockSidebar } from "../modes/SmallClock";
import { Main as ModeSmoothWaveformMain, name as ModeSmoothWaveformName } from "../modes/SmoothWaveform";
import { Main as ModeSnakeMain, name as ModeSnakeName, Sidebar as ModeSnakeSidebar } from "../modes/Snake";
import { Main as ModeStarsMain, name as ModeStarsName } from "../modes/Stars";
import { Main as ModeStreamMain, name as ModeStreamName, Sidebar as ModeStreamSidebar } from "../modes/Stream";
import { Main as ModeTickerMain, name as ModeTickerName, Sidebar as ModeTickerSidebar } from "../modes/Ticker";
import { Main as ModeWaveformMain, name as ModeWaveformName } from "../modes/Waveform";
import { Main as ModeWorldWeatherOnlineMain, name as ModeWorldWeatherOnlineName } from "../modes/WorldWeatherOnline";
import { Main as ModeWttrInMain, name as ModeWttrInName } from "../modes/WttrIn";
import { Main as ModeYrMain, name as ModeYrName } from "../modes/Yr";
import { DisplayPowerSet } from "./Display";

export const name = "Modes";

const [getMode, setMode] = createSignal<string>("");
const [getModes, setModes] = createSignal<string[]>([]);

export const ModesList = getModes;
export const ModesMode = getMode;

export const receiver = (json: { list?: string[]; mode?: string }) => {
    json?.list !== undefined && setModes(json.list);
    json?.mode !== undefined && setMode(json.mode);
};

export const Main: Component = () => (
    <Switch fallback={<MainComponent />}>
        {MODE_ANIMATION && (
            <Match when={getMode() === ModeAnimationName}>
                <ModeAnimationMain />
            </Match>
        )}
        {MODE_ARROW && (
            <Match when={getMode() === ModeArrowName}>
                <ModeArrowMain />
            </Match>
        )}
        {MODE_BINARYCLOCK && (
            <Match when={getMode() === ModeBinaryClockName}>
                <ModeBinaryClockMain />
            </Match>
        )}
        {MODE_BINARYEPOCH && (
            <Match when={getMode() === ModeBinaryEpochName}>
                <ModeBinaryEpochMain />
            </Match>
        )}
        {MODE_BLINDS && (
            <Match when={getMode() === ModeBlindsName}>
                <ModeBlindsMain />
            </Match>
        )}
        {MODE_BLINK && (
            <Match when={getMode() === ModeBlinkName}>
                <ModeBlinkMain />
            </Match>
        )}
        {MODE_BREAKOUTCLOCK && (
            <Match when={getMode() === ModeBreakoutClockName}>
                <ModeBreakoutClockMain />
            </Match>
        )}
        {MODE_BRIGHT && (
            <Match when={getMode() === ModeBrightName}>
                <ModeBrightMain />
            </Match>
        )}
        {MODE_CIRCLE && (
            <Match when={getMode() === ModeCircleName}>
                <ModeCircleMain />
            </Match>
        )}
        {MODE_COUNTDOWN && (
            <Match when={getMode() === ModeCountdownName}>
                <ModeCountdownMain />
            </Match>
        )}
        {MODE_DRAW && (
            <Match when={getMode() === ModeDrawName}>
                <ModeDrawMain />
            </Match>
        )}
        {MODE_EQUALIZER && (
            <Match when={getMode() === ModeEqualizerName}>
                <ModeEqualizerMain />
            </Match>
        )}
        {MODE_FIREWORK && (
            <Match when={getMode() === ModeFireworkName}>
                <ModeFireworkMain />
            </Match>
        )}
        {MODE_FLIES && (
            <Match when={getMode() === ModeFliesName}>
                <ModeFliesMain />
            </Match>
        )}
        {MODE_GAMEOFLIFE && (
            <Match when={getMode() === ModeGameOfLifeName}>
                <ModeGameOfLifeMain />
            </Match>
        )}
        {MODE_GLITTER && (
            <Match when={getMode() === ModeGlitterName}>
                <ModeGlitterMain />
            </Match>
        )}
        {MODE_GOOGLEWEATHER && (
            <Match when={getMode() === ModeGoogleWeatherName}>
                <ModeGoogleWeatherMain />
            </Match>
        )}
        {MODE_HOMEASSISTANTWEATHER && (
            <Match when={getMode() === ModeHomeAssistantWeatherName}>
                <ModeHomeAssistantWeatherMain />
            </Match>
        )}
        {MODE_HOMETHERMOMETER && (
            <Match when={getMode() === ModeHomeThermometerName}>
                <ModeHomeThermometerMain />
            </Match>
        )}
        {MODE_JAGGEDWAVEFORM && (
            <Match when={getMode() === ModeJaggedWaveformName}>
                <ModeJaggedWaveformMain />
            </Match>
        )}
        {MODE_LARGECLOCK && (
            <Match when={getMode() === ModeLargeClockName}>
                <ModeLargeClockMain />
            </Match>
        )}
        {MODE_LEAFFALL && (
            <Match when={getMode() === ModeLeafFallName}>
                <ModeLeafFallMain />
            </Match>
        )}
        {MODE_LINES && (
            <Match when={getMode() === ModeLinesName}>
                <ModeLinesMain />
            </Match>
        )}
        {MODE_METABALLS && (
            <Match when={getMode() === ModeMetaballsName}>
                <ModeMetaballsMain />
            </Match>
        )}
        {MODE_NOISE && (
            <Match when={getMode() === ModeNoiseName}>
                <ModeNoiseMain />
            </Match>
        )}
        {MODE_OPENMETEO && (
            <Match when={getMode() === ModeOpenMeteoName}>
                <ModeOpenMeteoMain />
            </Match>
        )}
        {MODE_OPENWEATHER && (
            <Match when={getMode() === ModeOpenWeatherName}>
                <ModeOpenWeatherMain />
            </Match>
        )}
        {MODE_PINGPONG && (
            <Match when={getMode() === ModePingPongName}>
                <ModePingPongMain />
            </Match>
        )}
        {MODE_PINGPONGCLOCK && (
            <Match when={getMode() === ModePingPongClockName}>
                <ModePingPongClockMain />
            </Match>
        )}
        {MODE_PIXELSEQUENCE && (
            <Match when={getMode() === ModePixelSequenceName}>
                <ModePixelSequenceMain />
            </Match>
        )}
        {MODE_RAIN && (
            <Match when={getMode() === ModeRainName}>
                <ModeRainMain />
            </Match>
        )}
        {MODE_RING && (
            <Match when={getMode() === ModeRingName}>
                <ModeRingMain />
            </Match>
        )}
        {MODE_SCAN && (
            <Match when={getMode() === ModeScanName}>
                <ModeScanMain />
            </Match>
        )}
        {MODE_SMALLCLOCK && (
            <Match when={getMode() === ModeSmallClockName}>
                <ModeSmallClockMain />
            </Match>
        )}
        {MODE_SMOOTHWAVEFORM && (
            <Match when={getMode() === ModeSmoothWaveformName}>
                <ModeSmoothWaveformMain />
            </Match>
        )}
        {MODE_SNAKE && (
            <Match when={getMode() === ModeSnakeName}>
                <ModeSnakeMain />
            </Match>
        )}
        {MODE_STARS && (
            <Match when={getMode() === ModeStarsName}>
                <ModeStarsMain />
            </Match>
        )}
        {MODE_STREAM && (
            <Match when={getMode() === ModeStreamName}>
                <ModeStreamMain />
            </Match>
        )}
        {MODE_TICKER && (
            <Match when={getMode() === ModeTickerName}>
                <ModeTickerMain />
            </Match>
        )}
        {MODE_WAVEFORM && (
            <Match when={getMode() === ModeWaveformName}>
                <ModeWaveformMain />
            </Match>
        )}
        {MODE_WORLDWEATHERONLINE && (
            <Match when={getMode() === ModeWorldWeatherOnlineName}>
                <ModeWorldWeatherOnlineMain />
            </Match>
        )}
        {MODE_WTTRIN && (
            <Match when={getMode() === ModeWttrInName}>
                <ModeWttrInMain />
            </Match>
        )}
        {MODE_YR && (
            <Match when={getMode() === ModeYrName}>
                <ModeYrMain />
            </Match>
        )}
    </Switch>
);

export const Sidebar: Component = () => {
    const handleMode = (mode: string) => {
        setMode(mode);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    mode: mode,
                },
            }),
        );
        DisplayPowerSet(true);
    };

    return (
        <>
            <SidebarSection title="Mode">
                <select
                    class="w-full"
                    name={name}
                    onchange={(e) => handleMode(e.currentTarget.value)}
                    value={getMode()}
                >
                    <For each={getModes()}>{(mode) => <option>{mode}</option>}</For>
                </select>
            </SidebarSection>
            <Switch>
                {MODE_ANIMATION && (
                    <Match when={getMode() === ModeAnimationName}>
                        <ModeAnimationSidebar />
                    </Match>
                )}
                {MODE_COUNTDOWN && (
                    <Match when={getMode() === ModeCountdownName}>
                        <ModeCountdownSidebar />
                    </Match>
                )}
                {MODE_DRAW && (
                    <Match when={getMode() === ModeDrawName}>
                        <ModeDrawSidebar />
                    </Match>
                )}
                {MODE_GAMEOFLIFE && (
                    <Match when={getMode() === ModeGameOfLifeName}>
                        <ModeGameOfLifeSidebar />
                    </Match>
                )}
                {MODE_LARGECLOCK && (
                    <Match when={getMode() === ModeLargeClockName}>
                        <ModeLargeClockSidebar />
                    </Match>
                )}
                {MODE_SMALLCLOCK && (
                    <Match when={getMode() === ModeSmallClockName}>
                        <ModeSmallClockSidebar />
                    </Match>
                )}
                {MODE_SNAKE && (
                    <Match when={getMode() === ModeSnakeName}>
                        <ModeSnakeSidebar />
                    </Match>
                )}
                {MODE_STREAM && (
                    <Match when={getMode() === ModeStreamName}>
                        <ModeStreamSidebar />
                    </Match>
                )}
                {MODE_TICKER && (
                    <Match when={getMode() === ModeTickerName}>
                        <ModeTickerSidebar />
                    </Match>
                )}
            </Switch>
        </>
    );
};

export const SidebarSecondary: Component = () => (
    <SidebarSectionSecondary title={name}>
        <div class="space-y-3">
            {MODE_COUNTDOWN && <ModeCountdownActions />}
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
    <Switch fallback={<WebAppMain />}>
        {MODE_COUNTDOWN && (
            <Match when={WebAppPath() === `/${name.toLowerCase()}/${ModeCountdownName.toLowerCase()}`}>
                <ModeCountdownMain />
            </Match>
        )}
        {MODE_HOMETHERMOMETER && (
            <Match when={WebAppPath() === `/${name.toLowerCase()}/${ModeHomeThermometerName.toLowerCase().replace(/\s+/g, "-")}`}>
                <ModeHomeThermometerMainThird />
            </Match>
        )}
    </Switch>
);

export const SidebarThird: Component = () => (
    <SidebarSectionSecondary title={name}>
        {MODE_COUNTDOWN && <ModeCountdownLink />}
        {MODE_HOMETHERMOMETER && <ModeHomeThermometerLink />}
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
            href={`${props.internal ? `#/${name.toLowerCase()}/` : `https://github.com/VIPnytt/Frekvens/wiki/${name}#-`}${getMode().toLowerCase().replace(/\s+/g, "-")}`}
            target={props.internal ? "_self" : "_blank"}
        >
            {getMode()}
        </a>
        {props.text && <div class="mode-subtitle">{props.text}</div>}
    </div>
);
