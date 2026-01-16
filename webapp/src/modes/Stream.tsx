import { mdiLaptop } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Stream';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLaptop}
        text={`Art-Net, Distributed Display Protocol or E1.31`}
    />
);
