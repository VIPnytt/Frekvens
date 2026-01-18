import { mdiGrain } from '@mdi/js';
import type { Component } from 'solid-js';

import { IkeaObegransad } from '../config/devices';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Noise';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiGrain}
        text={IkeaObegransad}
    />
);
