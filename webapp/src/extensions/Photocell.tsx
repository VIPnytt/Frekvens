import { mdiBrightnessAuto, mdiBrightnessPercent } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { Icon } from "../components/Icon";
import { Tooltip } from "../components/Tooltip";
import { name as ExtensionsName } from "../services/Extensions";
import { WebSocketWS } from "./WebSocket";

export const name = "Photocell";

const [getActive, setActive] = createSignal<boolean>(false);

export const PhotocellActive = getActive;

export const receiver = (json: { active?: boolean }) => {
    json?.active !== undefined && setActive(json.active);
};

const handleActive = () => {
    setActive(!getActive());
    WebSocketWS.send(
        JSON.stringify({
            [name]: {
                active: getActive(),
            },
        }),
    );
};

export const Actions: Component = () => (
    <div class="action">
        <a href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}>
            <Icon
                class="mr-2"
                path={mdiBrightnessAuto}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? "Deactivate" : "Activate"} ${name.toLowerCase()}`}>
            <button
                class={`w-full ${getActive() ? "action-activated" : "action-deactivated"}`}
                onclick={handleActive}
                type="button"
            >
                <Icon path={getActive() ? mdiBrightnessAuto : mdiBrightnessPercent} />
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
            path={mdiBrightnessAuto}
        />
        {name}
    </a>
);

export const MainThird: Component = () => (
    <div class="main">
        <div class="space-y-3 p-5">
            <h2>{name}</h2>
            <div class="box">
                <div class="space-y-3">
                    <h3>Ambient brightness adaption</h3>
                    <div class="text-sm">Automatically dims the display for comfortable viewing.</div>
                    <button
                        class={`mt-3 w-full ${getActive() ? "action-activated" : "action-deactivated"}`}
                        onclick={handleActive}
                        type="button"
                    >
                        <Icon
                            class="mr-2"
                            path={getActive() ? mdiBrightnessAuto : mdiBrightnessPercent}
                        />
                        {getActive() ? "enabled" : "disabled"}
                    </button>
                </div>
            </div>
        </div>
    </div>
);
