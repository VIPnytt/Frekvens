import { mdiBasketball, mdiCircleExpand, mdiSpeedometer } from "@mdi/js";
import { type Component, createSignal } from "solid-js";
import { Icon } from "../components/Icon";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";
import { Tooltip } from "../components/Tooltip";

export const name = "Metaballs";

const [getSpeed, setSpeed] = createSignal<number>(1);
const [getRadius, setRadius] = createSignal<number>(5);

export const receiver = (json: { speed?: number; radius?: number }) => {
    json?.speed !== undefined && setSpeed(json.speed);
    json?.radius !== undefined && setRadius(json.radius);
};

export const Main: Component = () => <ModesMainComponent icon={mdiBasketball} />;

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
                <Icon path={mdiSpeedometer} />
                <Tooltip text={`Metaball speed ${getSpeed()}`}>
                    <input
                        class="w-full"
                        max="10"
                        min="1"
                        onInput={(e) => handleSpeed(e.currentTarget.valueAsNumber)}
                        type="range"
                        value={getSpeed()}
                    />
                </Tooltip>
            </div>
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Icon path={mdiCircleExpand} />
                <Tooltip text={`Metaball radius ${getRadius()}`}>
                    <input
                        class="w-full"
                        max="10"
                        min="1"
                        onInput={(e) => handleRadius(e.currentTarget.valueAsNumber)}
                        type="range"
                        value={getRadius()}
                    />
                </Tooltip>
            </div>
        </SidebarSection>
    );
};
