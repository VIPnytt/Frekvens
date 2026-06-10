import { mdiMatrix } from "@mdi/js";
import { type Component, createSignal, onCleanup, onMount } from "solid-js";
import { Temporal } from "temporal-polyfill-lite";

import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Binary epoch";

export const Main: Component = () => {
    const [getTime, setTime] = createSignal(0);

    let timeout: ReturnType<typeof setTimeout>;

    const tick = () => {
        const now = Temporal.Now.instant();
        setTime(Math.floor(now.epochMilliseconds / 1_000));
        timeout = setTimeout(tick, 1_000 - (now.epochMilliseconds % 1_000));
    };

    onMount(tick);

    onCleanup(() => clearTimeout(timeout));

    return (
        <ModesMainComponent
            icon={mdiMatrix}
            text={getTime().toLocaleString()}
        />
    );
};
