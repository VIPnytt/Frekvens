import { mdiOneUp } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { ClockText } from "../components/Clock";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Game of Life";

const [getClock, setClock] = createSignal<boolean>(true);

export const receiver = (json: { clock?: boolean }) => {
    json?.clock !== undefined && setClock(json.clock);
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiOneUp}
        text={getClock() ? ClockText() : undefined}
    />
);

export const Sidebar: Component = () => {
    const handleClock = (clock: boolean) => {
        setClock(clock);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    clock: getClock(),
                },
            }),
        );
    };

    return (
        <SidebarSection>
            <label class="flex items-center gap-3 cursor-pointer">
                <input
                    type="checkbox"
                    checked={getClock()}
                    onChange={(e) => handleClock(e.currentTarget.checked)}
                    class="cursor-pointer w-5 h-5"
                />
                <span>Clock</span>
            </label>
        </SidebarSection>
    );
};
