import { mdiFormatText } from "@mdi/js";
import { type Component, createSignal, For } from "solid-js";

import { Tooltip } from "../components/Tooltip";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { FontsList } from "../services/Fonts";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Ticker";

const [getFont, setFont] = createSignal<string>("");
const [getMessage, setMessage] = createSignal<string>("");

export const receiver = (json: { font?: string; message?: string }) => {
    json?.font !== undefined && setFont(json.font);
    json?.message !== undefined && setMessage(json.message);
};

export const Main: Component = () => <ModesMainComponent icon={mdiFormatText} />;

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

    const handleMessage = (message: string) => {
        setMessage(message);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    message: getMessage(),
                },
            }),
        );
    };

    return (
        <SidebarSection>
            <div class="text-xs font-semibold uppercase text-content-alt-light dark:text-content-alt-dark">Message</div>
            <input
                class="mt-1 w-full"
                maxlength="255"
                onchange={(e) => handleMessage(e.currentTarget.value)}
                placeholder="Message"
                spellcheck="true"
                type="text"
                value={getMessage()}
            />
            <div class="mt-3 text-xs font-semibold uppercase text-content-alt-light dark:text-content-alt-dark">
                Font
            </div>
            <Tooltip text="Character availability may vary">
                <select
                    class="mt-1 w-full"
                    value={getFont()}
                    onchange={(e) => handleFont(e.currentTarget.value)}
                >
                    <For each={FontsList()}>{(font) => <option>{font}</option>}</For>
                </select>
            </Tooltip>
        </SidebarSection>
    );
};
