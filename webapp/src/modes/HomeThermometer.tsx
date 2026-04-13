import { mdiHomeThermometer } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { Icon } from "../components/Icon";
import { Tooltip } from "../components/Tooltip";
import { TEMPERATURE_UNIT } from "../config/constants";
import { EXTENSION_HOMEASSISTANT, EXTENSION_MQTT, EXTENSION_RESTFUL } from "../config/modules";
import { name as ExtensionHomeAssistantName } from "../extensions/HomeAssistant";
import { name as ExtensionMqttName, MqttTopic } from "../extensions/Mqtt";
import { name as ExtensionRestfulName, RestfulUrl } from "../extensions/Restful";
import { name as ExtensionWebSocketName, WebSocketUrl } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent, name as ModesName } from "../services/Modes";

export const name = "Home thermometer";

const [getIndoor, setIndoor] = createSignal<number | undefined>(undefined);
const [getOutdoor, setOutdoor] = createSignal<number | undefined>(undefined);

export const receiver = (json: { indoor?: number; outdoor?: number }) => {
    json?.indoor !== undefined && setIndoor(json.indoor);
    json?.outdoor !== undefined && setOutdoor(json.outdoor);
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiHomeThermometer}
        internal={true}
        text={
            getIndoor() === undefined || getOutdoor() === undefined
                ? "Smart-home integration"
                : `${getIndoor()}${TEMPERATURE_UNIT || "°"} / ${getOutdoor()}${TEMPERATURE_UNIT || "°"}`
        }
    />
);

export const Link: Component = () => (
    <Tooltip text="Smart-home thermometer">
        <a
            class="link"
            href={`#/${ModesName.toLowerCase()}/${name.toLowerCase().replace(/\s+/g, "-")}`}
        >
            <Icon
                class="mr-2"
                path={mdiHomeThermometer}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainSecondary: Component = () => {
    const fallback = (() => {
        switch (TEMPERATURE_UNIT) {
            case "°C":
                return {
                    indoor: 20,
                    outdoor: 10,
                };
            case "°F":
                return {
                    indoor: 70,
                    outdoor: 50,
                };
            case "°K":
                return {
                    indoor: 295,
                    outdoor: 280,
                };
        }
    })();

    const payload = `{"indoor": ${getIndoor() ?? fallback?.indoor ?? 0}, "outdoor": ${getOutdoor() ?? fallback?.outdoor ?? 0}}`;

    return (
        <div class="main">
            <div class="space-y-3 p-5">
                <h2>{name}</h2>
                <div class="box">
                    <div class="space-y-3">
                        <div class="text-sm mb-3">
                            <a
                                href={`https://github.com/VIPnytt/Frekvens/wiki/${ModesName}#-${name.toLowerCase().replace(/\s+/g, "-")}`}
                                rel="noopener"
                                target="_blank"
                            >
                                Get started by configuring your <span class="italic">smart-home system</span> to send
                                temperature readings.
                            </a>
                        </div>
                        {EXTENSION_HOMEASSISTANT && (
                            <>
                                <div class="border-t" />
                                <h3>{ExtensionHomeAssistantName}</h3>
                                <div class="text-sm">
                                    <span class="font-medium ">Automation:</span>{" "}
                                    <span class="text-content-alt-light dark:text-content-alt-dark">
                                        Set up in the user-interface.
                                    </span>
                                </div>
                            </>
                        )}
                        {EXTENSION_MQTT && (
                            <>
                                <div class="border-t" />
                                <h3>{ExtensionMqttName}</h3>
                                <div class="text-sm">
                                    <span class="font-medium ">Topic:</span>{" "}
                                    <span class="text-content-alt-light dark:text-content-alt-dark font-mono whitespace-nowrap">
                                        {MqttTopic}/set
                                    </span>
                                    <br />
                                    <span class="font-medium ">Message:</span>{" "}
                                    <span class="text-content-alt-light dark:text-content-alt-dark font-mono whitespace-nowrap">
                                        {payload}
                                    </span>
                                </div>
                            </>
                        )}
                        {EXTENSION_RESTFUL && (
                            <>
                                <div class="border-t" />
                                <h3>{ExtensionRestfulName}</h3>
                                <div class="text-sm">
                                    <span class="font-medium ">Method:</span> <span class="font-mono">PATCH</span>
                                    <br />
                                    <span class="font-medium ">URL:</span>{" "}
                                    <span class="text-content-alt-light dark:text-content-alt-dark font-mono">
                                        {RestfulUrl + encodeURIComponent(name)}
                                    </span>
                                    <br />
                                    <span class="font-medium ">Body:</span>{" "}
                                    <span class="text-content-alt-light dark:text-content-alt-dark font-mono whitespace-nowrap">
                                        {payload}
                                    </span>
                                </div>
                            </>
                        )}
                        <div class="border-t" />
                        <h3>{ExtensionWebSocketName}</h3>
                        <div class="text-sm">
                            <span class="font-medium ">URL:</span>{" "}
                            <span class="text-content-alt-light dark:text-content-alt-dark font-mono">
                                {WebSocketUrl}
                            </span>
                            <br />
                            <span class="font-medium ">Message:</span>{" "}
                            <span class="text-content-alt-light dark:text-content-alt-dark font-mono whitespace-nowrap">
                                {`{"${name}": ${payload}}`}
                            </span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};
