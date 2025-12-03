import { mdiLaptop } from '@mdi/js';
import { Component } from 'solid-js';

import { HOSTNAME } from '../config/constants';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'E1.31';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLaptop}
        text={`${HOSTNAME}.local:5568`}
    />
);
