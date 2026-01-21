import { type Component, createSignal, For } from "solid-js";

import { ClockIcon } from "../components/Clock";
import { Toast } from "../components/Toast";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Large clock";

const [getFont, setFont] = createSignal<string>("");
const [getFonts, setFonts] = createSignal<string[]>([]);
const [getTicking, setTicking] = createSignal<boolean>(false);

export const receiver = (json: { font?: string; fonts?: string[]; ticking?: boolean }) => {
    json?.font !== undefined && setFont(json.font);
    json?.fonts !== undefined && setFonts(json.fonts);
    json?.ticking !== undefined && setTicking(json.ticking);
};

const { toast } = Toast();

export const Main: Component = () => <ModesMainComponent icon={ClockIcon()} />;

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
        toast(`${name} updated`);
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
        toast(`${name} updated`);
    };

    return (
        <SidebarSection title={name}>
            <select
                class="mt-3 w-full"
                value={getFont()}
                onchange={(e) => handleFont(e.currentTarget.value)}
            >
                <For each={getFonts()}>{(font) => <option>{font}</option>}</For>
            </select>
            <label class="flex items-center gap-3 cursor-pointer">
                <input
                    type="checkbox"
                    checked={getTicking()}
                    onChange={(e) => handleTicking(e.currentTarget.checked)}
                    class="cursor-pointer w-5 h-5"
                />
                <span>Ticking</span>
            </label>
        </SidebarSection>
    );
};
