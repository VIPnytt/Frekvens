import {
    mdiAlertOutline,
    mdiWeatherCloudy,
    mdiWeatherFog,
    mdiWeatherLightning,
    mdiWeatherPartlyCloudy,
    mdiWeatherRainy,
    mdiWeatherSnowy,
    mdiWeatherSunny,
    mdiWeatherWindy,
} from "@mdi/js";
import { type Component, createSignal, For } from "solid-js";

import { TEMPERATURE_UNIT } from "../config/constants";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Weather";

const [getCondition, setCondition] = createSignal<string | undefined>(undefined);
const [getProvider, setProvider] = createSignal<string>("");
const [getProviders, setProviders] = createSignal<string[]>([]);
const [getTemperature, setTemperature] = createSignal<number | undefined>(undefined);

export const receiver = (json: {
    condition?: string;
    provider?: string;
    providers?: string[];
    temperature?: number;
}) => {
    json?.condition !== undefined && setCondition(json.condition);
    json?.provider !== undefined && setProvider(json.provider);
    json?.providers !== undefined && setProviders(json.providers);
    json?.temperature !== undefined && setTemperature(json.temperature);
};

const icons: Record<string, string> = {
    Clear: mdiWeatherSunny,
    Cloudy: mdiWeatherCloudy,
    Exception: mdiAlertOutline,
    Fog: mdiWeatherFog,
    Rain: mdiWeatherRainy,
    Snow: mdiWeatherSnowy,
    Thunder: mdiWeatherLightning,
    Wind: mdiWeatherWindy,
    "Partly cloudy": mdiWeatherPartlyCloudy,
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={icons[getCondition() ?? ""]}
        text={getTemperature() === undefined ? undefined : `${getTemperature()}${TEMPERATURE_UNIT || "°"}`}
    />
);

export const Sidebar: Component = () => {
    const handleProvider = (provider: string) => {
        setProvider(provider);
        setTemperature(undefined);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    provider: getProvider(),
                },
            }),
        );
    };

    return (
        <SidebarSection title="Provider">
            <select
                class="w-full"
                value={getProvider()}
                onchange={(e) => handleProvider(e.currentTarget.value)}
            >
                <For each={getProviders()}>{(provider) => <option>{provider}</option>}</For>
            </select>
        </SidebarSection>
    );
};
