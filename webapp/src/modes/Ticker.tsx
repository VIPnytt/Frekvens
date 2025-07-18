import { Component, createSignal, For } from 'solid-js';

import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { ws } from '../extensions/WebSocket';
import { FontsList } from '../services/Fonts';
import { SidebarSection } from '../services/WebServer';

export const name = 'Ticker';

const [getFont, setFont] = createSignal<string>('');
const [getMessage, setMessage] = createSignal<string>('');

export const receiver = (json: any) => {
    json[name]?.font !== undefined && setFont(json[name].font);
    json[name]?.message !== undefined && setMessage(json[name].message);
};

const { toast } = Toast();

export const Sidebar: Component = () => {
    const handleFont = (font: string) => {
        setFont(font);
        ws.send(JSON.stringify({
            [name]:
            {
                'font': getFont(),
            },
        }));
        toast(`${name} updated`);
    };

    const handleMessage = (message: string) => {
        setMessage(message);
        ws.send(JSON.stringify({
            [name]:
            {
                'message': getMessage(),
            },
        }));
        toast(`${name} updated`);
    };

    return (
        <SidebarSection title={name}>
            <input
                class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                maxlength="255"
                onchange={(e) =>
                    handleMessage(e.currentTarget.value)
                }
                placeholder="Message"
                spellcheck="true"
                type="text"
                value={getMessage()}
            />
            <Tooltip text="Character availability may vary">
                <select
                    class="w-full px-2.5 py-2.5 bg-gray-50 border border-gray-200 rounded"
                    value={getFont()}
                    onchange={(e) =>
                        handleFont(e.currentTarget.value)
                    }
                >
                    <For each={FontsList()}>
                        {
                            (font) => <option>{font}</option>
                        }
                    </For>
                </select>
            </Tooltip>
        </SidebarSection>
    );
};

export default Sidebar;
