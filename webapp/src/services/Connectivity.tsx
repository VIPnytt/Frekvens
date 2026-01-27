import { mdiContentSave, mdiWifi, mdiWifiRefresh, mdiWifiStrength1, mdiWifiStrength1Lock, mdiWifiStrength2, mdiWifiStrength2Lock, mdiWifiStrength3, mdiWifiStrength3Lock, mdiWifiStrength4, mdiWifiStrength4Lock, mdiWifiStrengthLockOutline, mdiWifiStrengthOutline } from "@mdi/js";
import { type Component, createSignal, For, Index } from "solid-js";

import { Icon } from "../components/Icon";
import { Toast } from "../components/Toast";
import { SidebarSectionSecondary, WebAppSidebarSet } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";

export const name = "Connectivity";

interface WiFi {
    rssi: number;
    ssid: string;
}

const [getConnecting, setConnecting] = createSignal<boolean>(false);
const [getCredentialKey, setCredentialKey] = createSignal<string>("");
const [getCredentialSsid, setCredentialSsid] = createSignal<string>("");
const [getScan, setScan] = createSignal<WiFi[]>([]);
const [getSaved, setSaved] = createSignal<string[]>([]);
const [getSsid, setSsid] = createSignal<string | undefined>(undefined);

export const receiver = (json: { event?: string; saved?: string[]; scan?: WiFi[]; ssid?: string | undefined }) => {
    json?.event !== undefined && event(json.event);
    json?.saved !== undefined && setSaved(json.saved);
    json?.scan !== undefined && setScan(json.scan);
    json?.ssid !== undefined && setSsid(json.ssid);
};

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case "connected":
            toast("Wi-Fi connected", 15e3);
            break;
    }
};

export const MainThird: Component = () => {
    const handleConnect = () => {
        WebSocketWS.send(
            JSON.stringify({
                [name]: getCredentialKey().length
                    ? {
                          key: getCredentialKey(),
                          ssid: getCredentialSsid(),
                      }
                    : {
                          ssid: getCredentialSsid(),
                      },
            }),
        );
        toast("Connecting to Wi-Fi...", 60e3);
        setConnecting(true);
    };

    return (
        <div class="main">
            <div class="space-y-3 p-5">
                <h2>Wi-Fi</h2>
                <div class="box">
                    <div class="space-y-2">
                        <div class="grid gap-3">
                            <h3>SSID</h3>
                            <input
                                class="w-full"
                                autofocus
                                autocomplete="username"
                                disabled={getConnecting()}
                                id="ssid"
                                list="scan"
                                oninput={(e) => setCredentialSsid(e.currentTarget.value)}
                                placeholder={getSsid() ?? "Name"}
                                type="text"
                                value={getCredentialSsid()}
                            />
                            <datalist id="scan">
                                <For each={getScan().filter((scan) => scan.ssid?.length)}>{(scan) => <option value={scan.ssid}></option>}</For>
                            </datalist>
                            <h3>Encryption key</h3>
                            <input
                                class="w-full"
                                autocomplete="current-password"
                                disabled={getConnecting()}
                                id="key"
                                oninput={(e) => setCredentialKey(e.currentTarget.value)}
                                placeholder={!getCredentialSsid() && getSsid() ? "********" : "Password"}
                                type="password"
                            />
                        </div>
                        <button
                            class={`action-deactivated mt-3 w-full ${getCredentialKey().length >= 8 ? "action-positive" : ""}`}
                            disabled={getConnecting() || !getCredentialSsid().length || (getCredentialKey().length > 0 && getCredentialKey().length < 8)}
                            id="connect"
                            onclick={handleConnect}
                            type="button"
                        >
                            <Icon
                                class="mr-2"
                                path={mdiContentSave}
                            />
                            {getConnecting() ? "Connecting" : "Connect"}
                        </button>
                    </div>
                </div>
            </div>
        </div>
    );
};

export const SidebarThird: Component = () => {
    const handleSelect = (ssid: string) => {
        if (window.innerWidth < 800) {
            WebAppSidebarSet(false);
        }
        setCredentialSsid(ssid);
        (document.getElementById("key") as HTMLInputElement).focus();
    };

    if (!getScan.length) {
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    action: "scan",
                },
            }),
        );
    }

    return (
        <SidebarSectionSecondary title="Networks">
            {getScan().length === 0 ? (
                <div class="link">
                    <Icon
                        class="mr-2"
                        path={mdiWifiRefresh}
                    />
                    Scanning...
                </div>
            ) : (
                <Index each={getScan()}>
                    {(item) => (
                        <div
                            class="cursor-pointer link space-y-2"
                            onclick={() => handleSelect(item().ssid)}
                        >
                            <Icon
                                class="mr-2"
                                path={
                                    item().rssi >= -45
                                        ? getSaved().includes(item().ssid)
                                            ? mdiWifiStrength4
                                            : mdiWifiStrength4Lock
                                        : item().rssi >= -55
                                          ? getSaved().includes(item().ssid)
                                              ? mdiWifiStrength3
                                              : mdiWifiStrength3Lock
                                          : item().rssi >= -65
                                            ? getSaved().includes(item().ssid)
                                                ? mdiWifiStrength2
                                                : mdiWifiStrength2Lock
                                            : item().rssi >= -75
                                              ? getSaved().includes(item().ssid)
                                                  ? mdiWifiStrength1
                                                  : mdiWifiStrength1Lock
                                              : getSaved().includes(item().ssid)
                                                ? mdiWifiStrengthOutline
                                                : mdiWifiStrengthLockOutline
                                }
                            />
                            {item().ssid}
                        </div>
                    )}
                </Index>
            )}
        </SidebarSectionSecondary>
    );
};

export const SidebarSecondaryComponent: Component = () => (
    <a
        class="link"
        href={`#/${name.toLowerCase()}`}
    >
        <Icon
            class="mr-2"
            path={mdiWifi}
        />
        {getSsid() ?? "Wi-Fi"}
    </a>
);
