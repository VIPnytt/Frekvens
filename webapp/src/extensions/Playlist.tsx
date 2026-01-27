import { mdiDelete, mdiPlay, mdiPlaylistPlay, mdiPlus, mdiStop } from "@mdi/js";
import Cookies from "js-cookie";
import { type Component, createSignal, For, Index } from "solid-js";

import { Icon } from "../components/Icon";
import { Tooltip } from "../components/Tooltip";
import { name as ExtensionsName } from "../services/Extensions";
import { ModesList, name as ModesName } from "../services/Modes";
import { WebSocketWS } from "./WebSocket";

export const name = "Playlist";

interface Item {
    duration: number;
    mode: string;
}

const [getActive, setActive] = createSignal<boolean>(false);
const [getDuration, setDuration] = createSignal<number>(parseInt(Cookies.get(`${name}.duration`) ?? "", 10) || 60);
const [getPlaylist, setPlaylist] = createSignal<Item[]>([]);

export const PlaylistActive = getActive;

export const receiver = (json: { active?: boolean; playlist?: Item[] }) => {
    json?.active !== undefined && setActive(json.active);
    json?.playlist !== undefined && setPlaylist(json.playlist);
};

const handleActive = () => {
    setActive(!getActive());
    WebSocketWS.send(
        JSON.stringify({
            [name]: {
                active: getActive(),
                playlist: getPlaylist(),
            },
        }),
    );
};

export const Actions: Component = () => (
    <div class="action">
        <a href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}>
            <Icon
                class="mr-2"
                path={mdiPlaylistPlay}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? "Stop" : "Play"} ${name.toLowerCase()}`}>
            <button
                class={`w-full ${getActive() ? "action-activated" : "action-deactivated"}`}
                disabled={!getPlaylist().length}
                onclick={handleActive}
                type="button"
            >
                <Icon path={getActive() ? mdiStop : mdiPlay} />
            </button>
        </Tooltip>
    </div>
);

export const Link: Component = () => (
    <a
        class="link"
        href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
    >
        <Icon
            class="mr-2"
            path={mdiPlaylistPlay}
        />
        {name}
    </a>
);

export const MainThird: Component = () => {
    const handleAdd = () => {
        do {
            setPlaylist([
                ...getPlaylist(),
                {
                    duration: getDuration(),
                    mode: ModesList()[Math.round(Math.random() * (ModesList().length - 1))],
                },
            ]);
        } while (getPlaylist().length < 2);
    };

    const handleDuration = (index: number, duration: number) => {
        setDuration(duration);
        setPlaylist(
            getPlaylist().map((item, i) =>
                i === index
                    ? {
                          ...item,
                          duration,
                      }
                    : item,
            ),
        );
        Cookies.set(`${name}.duration`, getDuration().toString(), {
            expires: 365,
        });
    };

    const handleMode = (index: number, name: string) => {
        setPlaylist(
            getPlaylist().map((item, i) =>
                i === index
                    ? {
                          ...item,
                          mode: name,
                      }
                    : item,
            ),
        );
    };

    const handleRemove = (index: number) => {
        setPlaylist(getPlaylist().filter((_, i) => i !== index));
    };

    return (
        <div class="main">
            <div class="space-y-3 p-5">
                <h2>{name}</h2>
                <div class="box">
                    <div class="space-y-3">
                        {getPlaylist().length && (
                            <>
                                <h3>{ModesName}</h3>
                                <Index each={getPlaylist()}>
                                    {(item, index) => (
                                        <div class="flex items-center gap-3">
                                            <select
                                                class="w-full"
                                                disabled={getActive()}
                                                onChange={(e) => handleMode(index, e.currentTarget.value)}
                                                value={item().mode}
                                            >
                                                <For each={ModesList()}>{(mode) => <option>{mode}</option>}</For>
                                            </select>
                                            <div class="relative">
                                                <input
                                                    class="text-right pr-6 w-24"
                                                    disabled={getActive()}
                                                    max={2 ** 16 - 1}
                                                    min="10"
                                                    onInput={(e) => handleDuration(index, parseInt(e.currentTarget.value, 10))}
                                                    step="5"
                                                    type="number"
                                                    value={item().duration}
                                                />
                                                <span class="absolute text-content-alt-light dark:text-content-alt-dark right-3 top-1/2 -translate-y-1/2 text-sm">s</span>
                                            </div>
                                            <div class="flex-none">
                                                <button
                                                    class="action-negative"
                                                    disabled={getActive()}
                                                    onclick={() => handleRemove(index)}
                                                    type="button"
                                                >
                                                    <Icon path={mdiDelete} />
                                                </button>
                                            </div>
                                        </div>
                                    )}
                                </Index>
                            </>
                        )}
                        <div class="grid grid-cols-2 gap-3 mt-3">
                            <Tooltip text="Add mode">
                                <button
                                    class={`action-neutral w-full ${getPlaylist().length < 2 ? `bg-neutral-light dark:enabled:bg-neutral-dark text-interactive-light dark:text-content-dark` : ""}`}
                                    disabled={getActive()}
                                    onclick={handleAdd}
                                    type="button"
                                >
                                    <Icon path={mdiPlus} />
                                </button>
                            </Tooltip>
                            <Tooltip text={`${getActive() ? "Stop" : "Play"} ${name.toLowerCase()}`}>
                                <button
                                    class={`w-full ${getActive() ? "action-activated" : "action-positive"}`}
                                    disabled={getPlaylist().length < 2}
                                    onclick={handleActive}
                                    type="button"
                                >
                                    <Icon path={getActive() ? mdiStop : mdiPlay} />
                                </button>
                            </Tooltip>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};
