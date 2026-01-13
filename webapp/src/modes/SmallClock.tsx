import { type Component, createSignal } from "solid-js";

import { ClockIcon } from "../components/Clock";
import { Toast } from "../components/Toast";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Small clock";

const [getTicking, setTicking] = createSignal<boolean>(true);

export const receiver = (json: any) => {
    json[name]?.ticking !== undefined && setTicking(json[name].ticking);
};

const { toast } = Toast();

export const Main: Component = () => <ModesMainComponent icon={ClockIcon()} />;

export const Sidebar: Component = () => {
    const handleTicking = (ticking: boolean) => {
        setTicking(ticking);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    ticking: getTicking(),
                },
            }),
        );
        toast(`${name} updated`);
    };

    return (
        <SidebarSection title={name}>
            <label class="flex items-center gap-3 cursor-pointer">
                <input
                    type="checkbox"
                    checked={getTicking()}
                    onChange={(e) => handleTicking(e.currentTarget.checked)}
                    class="cursor-pointer w-5 h-5"
                />
                <span>Ticking</span>
            </label>
        </SidebarSection>
    );
};
