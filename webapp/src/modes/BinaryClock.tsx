import { mdiMatrix } from "@mdi/js";
import type { Component } from "solid-js";

import { ClockText } from "../components/Clock";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Binary clock";

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiMatrix}
        text={ClockText()}
    />
);
