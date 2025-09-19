import { mdiDelete, mdiPlay, mdiPlaylistPlay, mdiPlus, mdiStop } from '@mdi/js';
import Cookies from 'js-cookie';
import { Component, createSignal, For, Index } from 'solid-js';

import { Button } from '../components/Button';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from './WebSocket';
import { name as ExtensionsName } from '../services/Extensions';
import { ModesList, name as ModesName } from '../services/Modes';
import { SidebarSection } from '../services/WebServer';

export const name = 'Playlist';

interface Item {
    duration: number;
    mode: string;
}

const [getActive, setActive] = createSignal<boolean>(false);
const [getDuration, setDuration] = createSignal<number>(parseInt(Cookies.get(`${name}.duration`) ?? '', 10) || 60);
const [getPlaylist, setPlaylist] = createSignal<Item[]>([]);

export const PlaylistActive = getActive;

export const receiver = (json: any) => {
    json[name]?.playlist !== undefined && setPlaylist(json[name].playlist);
    json[name]?.active !== undefined && setActive(json[name].active);
};

const handleActive = () => {
    setActive(!getActive());
    ws.send(JSON.stringify({
        [name]: {
            active: getActive(),
            playlist: getPlaylist(),
        },
    }));
};

export const Sidebar: Component = () => (
    <SidebarSection title={name}>
        <div class="flex flex-col gap-3">
            <Button
                class="w-full hover:bg-red-600 text-white border-0 px-4 py-3 uppercase text-sm leading-6 tracking-wider cursor-pointer font-bold hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                onClick={handleActive}
            >
                <Icon path={mdiStop} />
            </Button>
        </div>
    </SidebarSection>
);

export const Actions: Component = () => (
    <div class="grid grid-cols-[1fr_48px] gap-3 items-center font-medium text-gray-700 hover:text-gray-900">
        <a href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}>
            <Icon
                class="mr-2"
                path={mdiPlaylistPlay}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? 'Stop' : 'Play'} ${name.toLowerCase()}`}>
            <Button
                class={`w-full bg-blue-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'hover:bg-red-600' : 'hover:bg-green-600'}`}
                disabled={!getPlaylist().length}
                onClick={handleActive}
            >
                <Icon path={getActive() ? mdiStop : mdiPlay} />
            </Button>
        </Tooltip>
    </div>
);

export const Link: Component = () => (
    <Tooltip text={`Switch between ${ModesName.toLowerCase()} automatically`}>
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiPlaylistPlay}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => {
    const handleAdd = () => {
        do {
            setPlaylist([
                ...getPlaylist(),
                {
                    duration: getDuration(),
                    mode: ModesList()[Math.floor(Math.random() * ModesList().length)],
                },
            ]);
        } while (getPlaylist().length < 2);
    };

    const handleDuration = (index: number, duration: number) => {
        setDuration(duration);
        setPlaylist(getPlaylist().map((item, i) =>
            i === index ? { ...item, duration } : item,
        ));
        Cookies.set(`${name}.duration`, getDuration().toString(), {
            expires: 365,
        });
    };

    const handleMode = (index: number, name: string) => {
        setPlaylist(getPlaylist().map((item, i) =>
            i === index ? { ...item, mode: name } : item,
        ));
    };

    const handleRemove = (index: number) => {
        setPlaylist(getPlaylist().filter((_, i) => i !== index));
    };

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    {getPlaylist().length && (
                        <>
                            <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">{ModesName}</h3>
                            <Index each={getPlaylist()}>
                                {(item, index) => (
                                    <div class="flex items-center gap-3">
                                        <select
                                            class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                                            disabled={getActive()}
                                            onChange={(e) =>
                                                handleMode(index, e.currentTarget.value)
                                            }
                                            value={item().mode}
                                        >
                                            <For each={ModesList()}>
                                                {
                                                    (mode) => <option>{mode}</option>
                                                }
                                            </For>
                                        </select>
                                        <div class="relative">
                                            <input
                                                class="text-right pr-16 pl-3 py-2 w-32 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                                                disabled={getActive()}
                                                max={Math.pow(2, 16) - 1}
                                                min="10"
                                                onInput={(e) =>
                                                    handleDuration(index, parseInt(e.currentTarget.value))
                                                }
                                                type="number"
                                                value={item().duration}
                                            />
                                            <span class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                                                seconds
                                            </span>
                                        </div>
                                        <div class="flex-1">
                                            <Button
                                                class="p-2 hover:bg-red-600 rounded-lg transition-all duration-200"
                                                disabled={getActive()}
                                                onClick={() => handleRemove(index)}
                                            >
                                                <Icon path={mdiDelete} />
                                            </Button>
                                        </div>
                                    </div>
                                )}
                            </Index>
                        </>
                    )}
                    <div class="grid grid-cols-2 gap-3">
                        <Tooltip text="Add mode">
                            <Button
                                class="hover:bg-green-600 transition-colors"
                                disabled={getActive()}
                                onClick={handleAdd}
                            >
                                <Icon path={mdiPlus} />
                            </Button>
                        </Tooltip>
                        <Tooltip text={`${getActive() ? 'Stop' : 'Play'} ${name.toLowerCase()}`}>
                            <Button
                                class={`border-0 px-4 py-3 uppercase leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded transition-colors ${getActive() ? 'bg-red-600' : 'hover:bg-green-600'}`}
                                disabled={getPlaylist().length < 2}
                                onClick={handleActive}
                            >
                                <Icon path={getActive() ? mdiStop : mdiPlay} />
                            </Button>
                        </Tooltip>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default MainThird;
