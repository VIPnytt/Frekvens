import { mdiFormatText } from '@mdi/js';
import { Component, createSignal, For } from 'solid-js';

import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { SidebarSection } from '../extensions/WebApp';
import { WebSocketWS } from '../extensions/WebSocket';
import { FontsList } from '../services/Fonts';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Ticker';

const [getFont, setFont] = createSignal<string>('');
const [getMessage, setMessage] = createSignal<string>('');

export const receiver = (json: any) => {
    json[name]?.font !== undefined && setFont(json[name].font);
    json[name]?.message !== undefined && setMessage(json[name].message);
};

const { toast } = Toast();

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiFormatText}
    />
);

export const Sidebar: Component = () => {
    const handleFont = (font: string) => {
        setFont(font);
        WebSocketWS.send(JSON.stringify({
            [name]:
            {
                'font': getFont(),
            },
        }));
        toast(`${name} updated`);
    };

    const handleMessage = (message: string) => {
        setMessage(message);
        WebSocketWS.send(JSON.stringify({
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
                class="w-full"
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
                    class="mt-3 w-full"
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
