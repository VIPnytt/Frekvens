import { mdiCircleDouble } from "@mdi/js";
import type { Component } from "solid-js";

import { IkeaObegransad } from "../config/devices";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Circle";

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiCircleDouble}
        text={IkeaObegransad}
    />
);
