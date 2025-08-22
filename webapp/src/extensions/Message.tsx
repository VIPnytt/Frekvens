import { mdiMessage, mdiSend } from '@mdi/js';
import Cookies from 'js-cookie';
import { Component, createSignal, For } from 'solid-js';

import { Button } from '../components/Button';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from './WebSocket';
import { DisplayColumns, name as DisplayName } from '../services/Display';
import { name as ExtensionsName } from '../services/Extensions';
import { FontsList } from '../services/Fonts';

export const name = 'Message';

const [getFont, setFont] = createSignal<string>('');
const [getMessage, setMessage] = createSignal<string>(Cookies.get(`${name}.message`) || '');
const [getRepeat, setRepeat] = createSignal<number>(0);

export const receiver = (json: any) => {
    json[name]?.event !== undefined && event(json[name].event);
    json[name]?.font !== undefined && setFont(json[name].font);
    json[name]?.repeat !== undefined && setRepeat(json[name].repeat);
};

const { toast } = Toast();

const event = (message: string) => {
    toast(message, Math.pow(2, 7) * (DisplayColumns() + message.length * 6.8));
};

export const Link: Component = () => (
    <Tooltip text="Rolling text on-demand">
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiMessage}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => {
    const handleSend = () => {
        ws.send(JSON.stringify({
            [name]:
            {
                'font': getFont(),
                'message': getMessage(),
                'repeat': getRepeat(),
            },
        }));
        Cookies.set(`${name}.message`, getMessage(), {
            expires: 365,
        });
    };

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Message</h3>
                    <input
                        autofocus
                        class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                        maxlength="255"
                        onchange={(e) =>
                            setMessage(e.currentTarget.value)
                        }
                        placeholder={name}
                        spellcheck="true"
                        type="text"
                        value={getMessage()}
                    />
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Repeat</h3>
                    <div class="relative">
                        <input
                            class="text-right w-full pr-14 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                            min="0"
                            max={Math.pow(2, 8) - 1}
                            onchange={(e) =>
                                setRepeat(parseInt(e.currentTarget.value))
                            }
                            type="number"
                            value={getRepeat()}
                        />
                        <span class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                            {getRepeat() === 1 ? 'time' : 'times'}
                        </span>
                    </div>
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">Font</h3>
                    <Tooltip text="Character availability may vary">
                        <select
                            class="w-full px-2.5 py-2.5 bg-gray-50 border border-gray-200 rounded"
                            onchange={(e) =>
                                setFont(e.currentTarget.value)
                            }
                            value={getFont()}
                        >
                            <For each={FontsList()}>
                                {
                                    (fontName) => <option selected={fontName == getFont()}>{fontName}</option>
                                }
                            </For>
                        </select>
                    </Tooltip>
                    <Tooltip text={`${DisplayName} ${name.toLowerCase()}`}>
                        <Button
                            class="hover:bg-green-600 transition-colors"
                            disabled={getMessage().length === 0}
                            onClick={handleSend}
                        >
                            <Icon
                                class="mr-2"
                                path={mdiSend}
                            />
                            {DisplayName}
                        </Button>
                    </Tooltip>
                </div>
            </div>
        </div>
    );
};

export default MainThird;
