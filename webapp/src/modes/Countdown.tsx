import { mdiTimerSandFull } from '@mdi/js';
import Cookies from 'js-cookie';
import { Component, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Toast } from '../components/Toast';
import { Icon } from '../components/Vector';
import { ws } from '../extensions/WebSocket';
import { SidebarSection } from '../services/WebServer';


export const name = 'Countdown';

const [getHours, setHours] = createSignal<number>(parseInt(Cookies.get(`${name}.hours`) || '', 10) || 0);
const [getMinutes, setMinutes] = createSignal<number>(parseInt(Cookies.get(`${name}.minutes`) || '', 10) || 10);
const [getSeconds, setSeconds] = createSignal<number>(parseInt(Cookies.get(`${name}.seconds`) || '', 10) || 0);
const [getTimestamp, setTimestamp] = createSignal<string | undefined>(undefined);

export const receiver = (json: any) => {
    json[name]?.event !== undefined && event(json[name].event);
    json[name]?.timestamp !== undefined && setTimestamp(json[name].timestamp);
};

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case 'done':
            toast(`${name} ended`, 60e3);
            break;
    }
};

export const Sidebar: Component = () => {
    const handleAbsolute = (timestamp: string) => {
        if (/^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}$/.test(timestamp)) {
            timestamp += ':00';
        }
        ws.send(JSON.stringify({
            [name]: {
                timestamp: timestamp,
            },
        }));
        toast(`${name} updated`);
    };

    return (
        <SidebarSection title={name}>
            <input
                class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200"
                type="datetime-local"
                min={new Date().toISOString().slice(0, 16)}
                value={getTimestamp()}
                onChange={(e) =>
                    handleAbsolute(e.currentTarget.value)
                }
            />
        </SidebarSection>
    );
};

export const Main: Component = () => {
    const handleRelative = () => {
        ws.send(JSON.stringify({
            [name]: {
                seconds: getHours() * 3600 + getMinutes() * 60 + getSeconds(),
            },
        }));
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

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <div class="grid gap-3">
                        <div class="flex items-center gap-3">
                            <div class="relative">
                                <input
                                    class="text-right pr-6 pl-3 py-2 w-15 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200"
                                    type="number"
                                    max="99"
                                    min="0"
                                    value={getHours()}
                                    oninput={(e) =>
                                        setHours(parseInt(e.currentTarget.value))
                                    }
                                />
                                <span class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                                    h
                                </span>
                            </div>
                            <div class="relative">
                                <input
                                    class="text-right pr-6 pl-3 py-2 w-15 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200"
                                    type="number"
                                    max="59"
                                    min="0"
                                    value={getMinutes()}
                                    oninput={(e) =>
                                        setMinutes(parseInt(e.currentTarget.value))
                                    }
                                />
                                <span class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                                    m
                                </span>
                            </div>
                            <div class="relative">
                                <input
                                    class="text-right pr-6 pl-3 py-2 w-15 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200"
                                    type="number"
                                    max="59"
                                    min="0"
                                    value={getSeconds()}
                                    oninput={(e) =>
                                        setSeconds(parseInt(e.currentTarget.value))
                                    }
                                />
                                <span class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                                    s
                                </span>
                            </div>
                        </div>
                    </div>
                    <Button
                        class="hover:bg-green-600 transition-colors"
                        onClick={handleRelative}
                    >
                        <Icon
                            class="mr-2"
                            path={mdiTimerSandFull}
                        />
                        Start
                    </Button>
                </div>
            </div>
        </div>
    );
};

export default Main;
