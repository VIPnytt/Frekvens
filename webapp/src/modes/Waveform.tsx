import { mdiWaveform } from "@mdi/js";
import { type Component, createSignal, For } from "solid-js";

import { Icon } from "../components/Icon";
import { IkeaFrekvens } from "../config/devices";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Waveform";

const [getWave, setWave] = createSignal<string>("");
const [getWaves, setWaves] = createSignal<string[]>([]);

export const receiver = (json: { wave?: string; waves?: string[] }) => {
    json?.wave !== undefined && setWave(json.wave);
    json?.waves !== undefined && setWaves(json.waves);
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWaveform}
        text={IkeaFrekvens}
    />
);

export const Sidebar: Component = () => {
    const handleVariant = (variant: string) => {
        setWave(variant);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    wave: getWave(),
                },
            }),
        );
    };

    return (
        <SidebarSection title="Wave">
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Icon path={mdiWaveform} />
                <select
                    class="w-full"
                    value={getWave()}
                    onchange={(e) => handleVariant(e.currentTarget.value)}
                >
                    <For each={getWaves()}>{(wave) => <option>{wave}</option>}</For>
                </select>
            </div>
        </SidebarSection>
    );
};
