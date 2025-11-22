import { mdiWeatherPartlyCloudy } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Open-Meteo';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWeatherPartlyCloudy}
    />
);
