import { mdiScanner } from '@mdi/js';
import { Component } from 'solid-js';

import { IkeaObegransad } from '../config/devices';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Scan';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiScanner}
        text={IkeaObegransad}
    />
);
