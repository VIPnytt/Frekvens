import { mdiCircleExpand } from "@mdi/js";
import type { Component } from "solid-js";

import { IkeaFrekvens } from "../config/devices";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Ring";

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiCircleExpand}
        text={IkeaFrekvens}
    />
);
