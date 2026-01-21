import { mdiTimerSand, mdiTimerSandComplete } from "@mdi/js";
import Cookies from "js-cookie";
import { type Component, createSignal } from "solid-js";

import { Icon } from "../components/Icon";
import { Toast } from "../components/Toast";
import { Tooltip } from "../components/Tooltip";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { name as DisplayName } from "../services/Display";
import { ModesMode, name as ModesName } from "../services/Modes";

export const name = "Countdown";

const [getHours, setHours] = createSignal<number>(parseInt(Cookies.get(`${name}.hours`) || "", 10) || 0);
const [getMinutes, setMinutes] = createSignal<number>(parseInt(Cookies.get(`${name}.minutes`) || "", 10) || 10);
const [getSeconds, setSeconds] = createSignal<number>(parseInt(Cookies.get(`${name}.seconds`) || "", 10) || 0);
const [getTimestamp, setTimestamp] = createSignal<string | undefined>(undefined);

export const receiver = (json: { event?: string; timestamp?: string | undefined }) => {
    json?.event !== undefined && event(json.event);
    json?.timestamp !== undefined && setTimestamp(json.timestamp);
};

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case "done":
            toast(`${name} ended`, 60e3);
            break;
    }
};

const handleRelative = () => {
    WebSocketWS.send(
        JSON.stringify({
            [name]: {
                time: getHours() * 3600 + getMinutes() * 60 + getSeconds(),
            },
            [DisplayName]: {
                power: true,
            },
            [ModesName]: {
                mode: name,
            },
        }),
    );
    toast(`${name} started`);
    Cookies.set(`${name}.hours`, getHours().toString(), {
        expires: 365,
    });
    Cookies.set(`${name}.minutes`, getMinutes().toString(), {
        expires: 365,
    });
    Cookies.set(`${name}.seconds`, getSeconds().toString(), {
        expires: 365,
    });
};

export const Actions: Component = () => (
    <div class="action">
        <a href={`#/${ModesName.toLowerCase()}/${name.toLowerCase()}`}>
            <Icon
                class="mr-2"
                path={mdiTimerSand}
            />
            {name}
        </a>
        <Tooltip text={`Start ${name.toLowerCase()}`}>
            <button
                class={`w-full ${ModesMode() === name ? "action-activated" : "action-deactivated"}`}
                disabled={ModesMode() === name}
                onclick={handleRelative}
                type="button"
            >
                <Icon path={ModesMode() === name ? mdiTimerSandComplete : mdiTimerSand} />
            </button>
        </Tooltip>
    </div>
);

export const Link: Component = () => (
    <Tooltip text="Countdown timer">
        <a
            class="link"
            href={`#/${ModesName.toLowerCase()}/${name.toLowerCase()}`}
        >
            <Icon
                class="mr-2"
                path={mdiTimerSand}
            />
            {name}
        </a>
    </Tooltip>
);

export const Sidebar: Component = () => {
    const handleAbsolute = (timestamp: string) => {
        if (/^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}$/.test(timestamp)) {
            timestamp += ":00";
        }
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    timestamp: timestamp,
                },
            }),
        );
        toast(`${name} updated`);
    };

    return (
        <SidebarSection title={name}>
            <input
                class="w-full"
                type="datetime-local"
                min={new Date().toISOString().slice(0, 16)}
                value={getTimestamp()}
                onChange={(e) => handleAbsolute(e.currentTarget.value)}
            />
        </SidebarSection>
    );
};

export const Main: Component = () => (
    <div class="main">
        <div class="space-y-3 p-5">
            <h2>{name}</h2>
            <div class="box">
                <div class="space-y-3">
                    <div class="grid gap-3">
                        <div class="flex items-center gap-3">
                            <div class="relative">
                                <input
                                    class="pr-6 text-right w-20"
                                    type="number"
                                    max="99"
                                    min="0"
                                    name="hours"
                                    value={getHours()}
                                    oninput={(e) => setHours(parseInt(e.currentTarget.value, 10))}
                                />
                                <span class="absolute text-content-alt-light dark:text-content-alt-dark right-3 top-1/2 -translate-y-1/2 text-sm">h</span>
                            </div>
                            <div class="relative">
                                <input
                                    class="pr-6 text-right w-20"
                                    type="number"
                                    max="59"
                                    min="0"
                                    name="minutes"
                                    value={getMinutes()}
                                    oninput={(e) => setMinutes(parseInt(e.currentTarget.value, 10))}
                                />
                                <span class="absolute text-content-alt-light dark:text-content-alt-dark right-3 top-1/2 -translate-y-1/2 text-sm">m</span>
                            </div>
                            <div class="relative">
                                <input
                                    class="pr-6 text-right w-20"
                                    type="number"
                                    max="59"
                                    min="0"
                                    name="seconds"
                                    value={getSeconds()}
                                    oninput={(e) => setSeconds(parseInt(e.currentTarget.value, 10))}
                                />
                                <span class="absolute text-content-alt-light dark:text-content-alt-dark right-3 top-1/2 -translate-y-1/2 text-sm">s</span>
                            </div>
                        </div>
                    </div>
                    <button
                        class="action-positive mt-3 w-full"
                        disabled={!getHours() && !getMinutes() && !getSeconds()}
                        onclick={handleRelative}
                        type="button"
                    >
                        <Icon
                            class="mr-2"
                            path={mdiTimerSand}
                        />
                        Start
                    </button>
                </div>
            </div>
        </div>
    </div>
);
