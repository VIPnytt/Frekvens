import { mdiLaptop } from '@mdi/js';
import { Component } from 'solid-js';

import { HOSTNAME } from '../config/constants';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Art-Net';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLaptop}
        text={`${HOSTNAME}.local:6454`}
    />
);
