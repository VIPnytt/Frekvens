import { mdiCircleHalfFull } from '@mdi/js';
import type { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Blink';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiCircleHalfFull}
    />
);
