import { mdiFormatFont, mdiFormatText } from "@mdi/js";
import { type Component, createSignal, For } from "solid-js";

import { Icon } from "../components/Icon";
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
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Icon path={mdiFormatText} />
                <input
                    class="mt-1 w-full"
                    maxlength="255"
                    onchange={(e) => handleMessage(e.currentTarget.value)}
                    placeholder="Message"
                    spellcheck="true"
                    type="text"
                    value={getMessage()}
                />
            </div>
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Icon path={mdiFormatFont} />
                <Tooltip text="Character availability may vary">
                    <select
                        class="w-full"
                        value={getFont()}
                        onchange={(e) => handleFont(e.currentTarget.value)}
                    >
                        <For each={FontsList()}>{(font) => <option>{font}</option>}</For>
                    </select>
                </Tooltip>
            </div>
        </SidebarSection>
    );
};
