import { mdiLaptop } from '@mdi/js';
import { type Component, createSignal } from "solid-js";

import { HOSTNAME } from '../config/constants';
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Stream';

const [getPort, setPort] = createSignal<number>(4048);

export const receiver = (json: any) => {
    json[name]?.port !== undefined && setPort(json[name].port);
};

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLaptop}
        text={`${HOSTNAME}.local:${getPort()}`}
    />
);

export const Sidebar: Component = () => {
    const handlePort = (port: number) => {
        setPort(port);
        WebSocketWS.send(JSON.stringify({
            [name]:
            {
                'port': getPort(),
            },
        }));
    };

    return (
        <SidebarSection title={name}>
            <select
                class="mt-3 w-full"
                value={getPort()}
                onchange={(e) =>
                    handlePort(parseInt(e.currentTarget.value, 10))
                }
            >
                <option value="6454">Art-Net</option>
                <option value="4048">Distributed Display Protocol</option>
                <option value="5568">E1.31</option>
            </select>
        </SidebarSection>
    );
};
