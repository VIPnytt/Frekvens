import { mdiWall } from "@mdi/js";
import type { Component } from "solid-js";

import { ClockText } from "../components/Clock";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Breakout clock";

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWall}
        text={ClockText()}
    />
);
