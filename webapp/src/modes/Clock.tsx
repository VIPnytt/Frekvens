import { type Component, createSignal, For } from "solid-js";

import { ClockIcon, ClockText } from "../components/Clock";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Clock";

const [getFont, setFont] = createSignal<string>("");
const [getFonts, setFonts] = createSignal<string[]>([]);
const [getTicking, setTicking] = createSignal<boolean>(false);

export const receiver = (json: { font?: string; fonts?: string[]; ticking?: boolean }) => {
    json?.font !== undefined && setFont(json.font);
    json?.fonts !== undefined && setFonts(json.fonts);
    json?.ticking !== undefined && setTicking(json.ticking);
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={ClockIcon()}
        text={ClockText()}
    />
);

export const Sidebar: Component = () => {
    const handleFont = (font: string) => {
        setFont(font);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    font: getFont(),
                },
            }),
        );
    };

    const handleTicking = (ticking: boolean) => {
        setTicking(ticking);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    ticking: getTicking(),
                },
            }),
        );
    };

    return (
        <SidebarSection>
            <div class="text-xs font-semibold uppercase text-content-alt-light dark:text-content-alt-dark">Font</div>
            <select
                class="mt-1 w-full"
                onchange={(e) => handleFont(e.currentTarget.value)}
                value={getFont()}
            >
                <For each={getFonts()}>{(font) => <option>{font}</option>}</For>
            </select>
            <label class="flex items-center gap-3">
                <input
                    checked={getTicking()}
                    onChange={(e) => handleTicking(e.currentTarget.checked)}
                    type="checkbox"
                />
                <span>Ticking</span>
            </label>
        </SidebarSection>
    );
};
