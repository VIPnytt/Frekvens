import { mdiOneUp } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Game of Life';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiOneUp}
    />
);
