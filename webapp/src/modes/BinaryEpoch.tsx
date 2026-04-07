import { mdiMatrix } from "@mdi/js";
import { type Component, createSignal, onCleanup } from "solid-js";

import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Binary epoch";

export const Main: Component = () => {
    const [getTime, setTime] = createSignal(Math.floor(Date.now() / 1000));

    let timeout: ReturnType<typeof setTimeout>;

    const tick = () => {
        setTime(Math.floor(Date.now() / 1000));
        timeout = setTimeout(tick, 1000 - (Date.now() % 1000));
    };

    tick();

    onCleanup(() => clearTimeout(timeout));

    return (
        <ModesMainComponent
            icon={mdiMatrix}
            text={getTime().toLocaleString()}
        />
    );
};
