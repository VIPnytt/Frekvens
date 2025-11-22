import { mdiWeatherPartlySnowyRainy } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Wttr.in';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWeatherPartlySnowyRainy}
    />
);
