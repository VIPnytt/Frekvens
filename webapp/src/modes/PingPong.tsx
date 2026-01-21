import { mdiTableTennis } from "@mdi/js";
import type { Component } from "solid-js";

import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Ping-Pong";

export const Main: Component = () => <ModesMainComponent icon={mdiTableTennis} />;
