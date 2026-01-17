import { mdiGrain } from '@mdi/js';
import type { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Glitter';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiGrain}
    />
);
