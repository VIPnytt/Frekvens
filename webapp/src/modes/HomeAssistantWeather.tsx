import { mdiHomeAssistant } from "@mdi/js";
import type { Component } from "solid-js";

import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Home Assistant weather";

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiHomeAssistant}
        text="Smart-home integration"
    />
);
