import { mdiMessage, mdiSend } from "@mdi/js";
import Cookies from "js-cookie";
import { type Component, createSignal, For } from "solid-js";

import { Icon } from "../components/Icon";
import { Toast } from "../components/Toast";
import { Tooltip } from "../components/Tooltip";
import { Device } from "../config/devices";
import { name as DisplayName } from "../services/Display";
import { name as ExtensionsName } from "../services/Extensions";
import { FontsList } from "../services/Fonts";
import { WebSocketWS } from "./WebSocket";

export const name = "Message";

const [getFont, setFont] = createSignal<string>("");
const [getMessage, setMessage] = createSignal<string>(Cookies.get(`${name}.message`) || "");
const [getRepeat, setRepeat] = createSignal<number>(0);

export const receiver = (json: { event?: string; font?: string; repeat?: number }) => {
    json?.event !== undefined && event(json.event);
    json?.font !== undefined && setFont(json.font);
    json?.repeat !== undefined && setRepeat(json.repeat);
};

const { toast } = Toast();

const event = (message: string) => {
    toast(message, 2 ** 7 * (Device.GRID_COLUMNS + message.length * 6.8));
};

export const Link: Component = () => (
    <a
        class="link"
        href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
    >
        <Icon
            class="mr-2"
            path={mdiMessage}
        />
        {name}
    </a>
);

export const MainThird: Component = () => {
    const handleSend = () => {
        WebSocketWS.send(
            JSON.stringify({
                [DisplayName]: {
                    power: true,
                },
                [name]: {
                    font: getFont(),
                    message: getMessage(),
                    repeat: getRepeat(),
                },
            }),
        );
        Cookies.set(`${name}.message`, getMessage(), {
            expires: 365,
        });
    };

    return (
        <div class="main">
            <div class="space-y-3 p-5">
                <h2>{name}</h2>
                <div class="box">
                    <div class="space-y-3">
                        <h3>Message</h3>
                        <input
                            autofocus
                            class="w-full"
                            maxlength="255"
                            onchange={(e) => setMessage(e.currentTarget.value)}
                            placeholder="Hello, World!"
                            spellcheck="true"
                            type="text"
                            value={getMessage()}
                        />
                        <h3>Repeat</h3>
                        <div class="relative">
                            <input
                                class="pr-14 text-right w-full"
                                min="0"
                                max={2 ** 8 - 1}
                                onchange={(e) => setRepeat(parseInt(e.currentTarget.value, 10))}
                                type="number"
                                value={getRepeat()}
                            />
                            <span class="absolute text-content-alt-light dark:text-content-alt-dark right-3 top-1/2 -translate-y-1/2 text-sm">
                                {getRepeat() === 1 ? "time" : "times"}
                            </span>
                        </div>
                        <h3>Font</h3>
                        <Tooltip text="Character availability may vary">
                            <select
                                class="w-full"
                                onchange={(e) => setFont(e.currentTarget.value)}
                                value={getFont()}
                            >
                                <For each={FontsList()}>
                                    {(fontName) => <option selected={fontName === getFont()}>{fontName}</option>}
                                </For>
                            </select>
                        </Tooltip>
                        <button
                            class="action-positive mt-3 w-full"
                            disabled={getMessage().length === 0}
                            onclick={handleSend}
                            type="button"
                        >
                            <Icon
                                class="mr-2"
                                path={mdiSend}
                            />
                            {DisplayName}
                        </button>
                    </div>
                </div>
            </div>
        </div>
    );
};
