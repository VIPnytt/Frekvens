import { mdiRouterWireless } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { Icon } from "../components/Icon";
import { HOSTNAME } from "../config/constants";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Stream";

const [getPort, setPort] = createSignal<number>(4048);

export const receiver = (json: { port?: number }) => {
    json?.port !== undefined && setPort(json.port);
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiRouterWireless}
        text={`${HOSTNAME}.local:${getPort()}`}
    />
);

export const Sidebar: Component = () => {
    const handlePort = (port: number) => {
        setPort(port);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    port: getPort(),
                },
            }),
        );
    };

    return (
        <SidebarSection title="Protocol">
            <div class="action grid-cols-[--spacing(4)_1fr]">
                <Icon path={mdiRouterWireless} />
                <select
                    class="w-full"
                    value={getPort()}
                    onchange={(e) => handlePort(parseInt(e.currentTarget.value, 10))}
                >
                    <option value="6454">Art-Net</option>
                    <option value="4048">Distributed Display Protocol</option>
                    <option value="5568">E1.31</option>
                </select>
            </div>
        </SidebarSection>
    );
};
