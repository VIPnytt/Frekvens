import { mdiBasketball, mdiCircleExpand, mdiSpeedometer } from "@mdi/js";
import { type Component, createSignal, For } from "solid-js";
import { Icon } from "../components/Icon";
import { Tooltip } from "../components/Tooltip";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Metaballs";

const [getSpeed, setSpeed] = createSignal<number>(1);
const [getRadius, setRadius] = createSignal<number>(5);

export const receiver = (json: { speed?: number; radius?: number }) => {
    json?.speed !== undefined && setSpeed(json.speed);
    json?.radius !== undefined && setRadius(json.radius);
};

export const Main: Component = () => <ModesMainComponent icon={mdiBasketball} />;

// Radius values the visualization actually renders sensibly.
const radiusOptions = [
    {
        value: 4,
        label: "Tiny",
    },
    {
        value: 5,
        label: "Small",
    },
    {
        value: 6,
        label: "Medium",
    },
    {
        value: 7,
        label: "Large",
    },
    {
        value: 8,
        label: "Huge",
    },
    {
        value: 9,
        label: "Massive",
    },
];

export const Sidebar: Component = () => {
    const handleSpeed = (speed: number) => {
        setSpeed(speed);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    speed: getSpeed(),
                },
            }),
        );
    };

    const handleRadius = (radius: number) => {
        setRadius(radius);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    radius: getRadius(),
                },
            }),
        );
    };

    return (
        <SidebarSection>
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Tooltip text={`"These go to eleven."`}>
                    <Icon path={mdiSpeedometer} />
                </Tooltip>
                <Tooltip text={`Metaball speed ${getSpeed()}`}>
                    <input
                        class="w-full"
                        max="11"
                        min="1"
                        onInput={(e) => handleSpeed(e.currentTarget.valueAsNumber)}
                        type="range"
                        value={getSpeed()}
                    />
                </Tooltip>
            </div>
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Icon path={mdiCircleExpand} />
                <Tooltip text={`Metaball radius`}>
                    <select
                        class="w-full"
                        onchange={(e) => handleRadius(Number(e.currentTarget.value))}
                        value={getRadius()}
                    >
                        <For each={radiusOptions}>{({ value, label }) => <option value={value}>{label}</option>}</For>
                    </select>
                </Tooltip>
            </div>
        </SidebarSection>
    );
};
