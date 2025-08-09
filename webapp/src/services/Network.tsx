import { mdiContentSave, mdiWifi, mdiWifiRefresh, mdiWifiStrength1, mdiWifiStrength1Lock, mdiWifiStrength2, mdiWifiStrength2Lock, mdiWifiStrength3, mdiWifiStrength3Lock, mdiWifiStrength4, mdiWifiStrength4Lock, mdiWifiStrengthLockOutline, mdiWifiStrengthOutline } from '@mdi/js';
import { Component, createSignal, For, Index } from 'solid-js';

import { Button } from '../components/Button';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from '../extensions/WebSocket';
import { SidebarSection } from './WebServer';

export const name = 'Network';

interface NetworkItem {
    bssid: string;
    rssi: number;
    ssid?: string | undefined;
}

const [getDomain, setDomain] = createSignal<string>(location.hostname);
const [getHostname, setHostname] = createSignal<string>(location.hostname.split('.')[0]);
const [getInputConnecting, setInputConnecting] = createSignal<boolean>(false);
const [getKeyNew, setKeyNew] = createSignal<string>('');
const [getScan, setScan] = createSignal<NetworkItem[]>([]);
const [getSaved, setSaved] = createSignal<string[]>([]);
const [getSsid, setSsid] = createSignal<string | undefined>(undefined);
const [getSsidNew, setSsidNew] = createSignal<string>('');

export const NetworkDomain = getDomain;
export const NetworkHostname = getHostname;

export const receiver = (json: any) => {
    json[name]?.domain !== undefined && setDomain(json[name].domain);
    json[name]?.event !== undefined && event(json[name].event);
    json[name]?.hostname !== undefined && setHostname(json[name].hostname);
    json[name]?.saved !== undefined && setSaved(json[name].saved);
    json[name]?.scan !== undefined && setScan(json[name].scan);
    json[name]?.ssid !== undefined && setSsid(json[name].ssid);
};

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case 'connected':
            toast('Wi-Fi connected', 15e3);
            break;
    }
};

export const MainThird: Component = () => {
    const handleConnect = () => {
        ws.send(JSON.stringify({
            [name]: getKeyNew().length ? {
                key: getKeyNew(),
                ssid: getSsidNew(),
            } : {
                ssid: getSsidNew(),
            },
        }));
        toast('Connecting to Wi-Fi...', 60e3);
        setInputConnecting(true);
    };

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">Wi-Fi</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <div class="grid gap-3">
                        <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                            SSID
                        </h3>
                        <input
                            class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                            autofocus
                            autocomplete="username"
                            disabled={getInputConnecting()}
                            id="ssid"
                            list="scan"
                            oninput={(e) =>
                                setSsidNew(e.currentTarget.value)
                            }
                            placeholder={getSsid() ?? 'Name'}
                            type="text"
                            value={getSsidNew()}
                        />
                        <datalist id="scan">
                            <For each={getScan().filter((scan) => scan.ssid?.length)}>
                                {
                                    (scan) => <option value={scan.ssid}></option>
                                }
                            </For>
                        </datalist>
                        <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                            Encryption key
                        </h3>
                        <input
                            class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                            autocomplete="current-password"
                            disabled={getInputConnecting()}
                            id="key"
                            oninput={(e) =>
                                setKeyNew(e.currentTarget.value)
                            }
                            placeholder={!getSsidNew() && getSsid() ? '********' : 'Password'}
                            type="password"
                        />
                    </div>
                    <Button
                        class="hover:bg-green-600 transition-colors"
                        disabled={getInputConnecting() || !getSsidNew().length || (getKeyNew().length > 0 && getKeyNew().length < 8)}
                        id="connect"
                        onClick={handleConnect}
                    >
                        <Icon
                            class="mr-2"
                            path={mdiContentSave}
                        />
                        {getInputConnecting() ? 'Connecting' : 'Connect'}
                    </Button>
                </div>
            </div>
        </div>
    );
};

export const SidebarThird: Component = () => {
    const handleScan = () => {
        (async () => {
            while (getScan().length === 0) {
                ws.send(JSON.stringify({
                    [name]: {
                        action: "scan",
                    },
                }));
                await new Promise(resolve => setTimeout(resolve, 5e3));
            }
        })();
    };

    const handleSelect = (ssid: string | undefined) => {
        if (ssid !== undefined) {
            setSsidNew(ssid);
        }
        (document.getElementById(ssid === undefined ? 'ssid' : 'key') as HTMLInputElement).focus();
    };

    if (!getScan.length) {
        handleScan();
    }

    return (
        <SidebarSection title="Networks">
            {getScan().length === 0 ? (
                <div class="py-3 inline-flex items-center text-gray-700 hover:text-gray-900 font-medium">
                    <Icon
                        class="mr-2"
                        path={mdiWifiRefresh}
                    />
                    Scanning...
                </div>
            ) : (
                <Index each={getScan()}>
                    {(item) => (
                        <div class="py-3">
                            <div
                                class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium cursor-pointer"
                                onClick={() => handleSelect(item().ssid)}
                            >
                                <Icon
                                    class="mr-2"
                                    path={item().rssi >= -45 ? (getSaved().includes(item().ssid ?? '') ? mdiWifiStrength4 : mdiWifiStrength4Lock) : item().rssi >= -55 ? (getSaved().includes(item().ssid ?? '') ? mdiWifiStrength3 : mdiWifiStrength3Lock) : item().rssi >= -65 ? (getSaved().includes(item().ssid ?? '') ? mdiWifiStrength2 : mdiWifiStrength2Lock) : item().rssi >= -75 ? (getSaved().includes(item().ssid ?? '') ? mdiWifiStrength1 : mdiWifiStrength1Lock) : (getSaved().includes(item().ssid ?? '') ? mdiWifiStrengthOutline : mdiWifiStrengthLockOutline)}
                                />
                                {item().ssid ?? (
                                    <span class="text-gray-300 italic">
                                        hidden
                                    </span>
                                )}
                            </div>
                        </div>
                    )}
                </Index>
            )}
        </SidebarSection>
    );
};

export const SidebarSecondary: Component = () => (
    <SidebarSection title={name}>
        <div class="space-y-2">
            <Tooltip text="Connect to Wi-Fi">
                <div class="grid gap-3 items-center font-medium text-gray-700 hover:text-gray-900 min-h-[48px]">
                    <a href={`#/${name.toLowerCase()}`}>
                        <Icon
                            class="mr-2"
                            path={mdiWifi}
                        />
                        {getSsid() ?? 'Wi-Fi'}
                    </a>
                </div>
            </Tooltip>
        </div>
    </SidebarSection>
);

export default MainThird;
