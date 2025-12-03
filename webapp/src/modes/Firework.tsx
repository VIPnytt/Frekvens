import { mdiFirework } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Firework';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiFirework}
    />
);
