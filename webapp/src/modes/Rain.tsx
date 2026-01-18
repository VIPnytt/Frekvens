import { mdiWeatherRainy } from '@mdi/js';
import type { Component } from 'solid-js';

import { IkeaObegransad } from '../config/devices';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Rain';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWeatherRainy}
        text={IkeaObegransad}
    />
);
