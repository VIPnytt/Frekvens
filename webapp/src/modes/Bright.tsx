import { mdiLightbulbOnOutline } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Bright';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLightbulbOnOutline}
    />
);
