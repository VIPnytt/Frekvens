import { mdiMicrophone, mdiMicrophoneOff } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { Icon } from "../components/Icon";
import { Tooltip } from "../components/Tooltip";
import { name as ExtensionsName } from "../services/Extensions";
import { WebSocketWS } from "./WebSocket";

export const name = "Microphone";

const [getActive, setActive] = createSignal<boolean>(false);
const [getCeiling, setCeiling] = createSignal<number>(0);
const [getFloor, setFloor] = createSignal<number>(0);
const [getThreshold, setThreshold] = createSignal<number>(0);

export const MicActive = getActive;

export const receiver = (json: { active?: boolean; ceiling?: number; floor?: number; threshold?: number }) => {
    json?.active !== undefined && setActive(json.active);
    json?.ceiling !== undefined && setCeiling(json.ceiling);
    json?.floor !== undefined && setFloor(json.floor);
    json?.threshold !== undefined && setThreshold(json.threshold);
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
                path={mdiMicrophone}
            />
            {name}
        </a>
        <Tooltip text={`${getActive() ? "Deactivate" : "Activate"} ${name.toLowerCase()}`}>
            <button
                class={`w-full ${getActive() ? "action-activated" : "action-deactivated"}`}
                onclick={handleActive}
                type="button"
            >
                <Icon path={getActive() ? mdiMicrophone : mdiMicrophoneOff} />
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
            path={mdiMicrophone}
        />
        {name}
    </a>
);

export const MainThird: Component = () => {
    const handleThreshold = (level: number, send: boolean = true) => {
        setThreshold(level);
        if (send) {
            WebSocketWS.send(
                JSON.stringify({
                    [name]: {
                        threshold: getThreshold(),
                    },
                }),
            );
        }
    };

    return (
        <div class="main">
            <div class="space-y-3 p-5">
                <h2>{name}</h2>
                <div class="box">
                    <div class="space-y-3">
                        <h3>Beat synchronization</h3>
                        <div class="text-sm">Some modes has the ability to react to sounds.</div>
                        <h3>Threshold</h3>
                        <Tooltip
                            text={`Threshold ${Math.ceil(((getThreshold() - getFloor()) / (getCeiling() - 1 - getFloor())) * 100)} %`}
                        >
                            <input
                                class="w-full"
                                type="range"
                                min={getFloor()}
                                max={getCeiling() - 1}
                                value={getThreshold()}
                                onInput={(e) => handleThreshold(e.currentTarget.valueAsNumber, false)}
                                onKeyUp={() => handleThreshold(getThreshold())}
                                onPointerUp={() => handleThreshold(getThreshold())}
                            />
                        </Tooltip>
                        <button
                            class={`mt-3 w-full ${getActive() ? "action-activated" : "action-deactivated"}`}
                            onclick={handleActive}
                            type="button"
                        >
                            <Icon
                                class="mr-2"
                                path={getActive() ? mdiMicrophone : mdiMicrophoneOff}
                            />
                            {getActive() ? "enabled" : "disabled"}
                        </button>
                    </div>
                </div>
            </div>
        </div>
    );
};
