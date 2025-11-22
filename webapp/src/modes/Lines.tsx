import { mdiTallyMark4 } from '@mdi/js';
import { Component } from 'solid-js';

import { IkeaObegransad } from '../config/devices';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Lines';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiTallyMark4}
        text={IkeaObegransad}
    />
);
