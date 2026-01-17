import { mdiEarth } from '@mdi/js';
import type { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'World Weather Online';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiEarth}
    />
);
