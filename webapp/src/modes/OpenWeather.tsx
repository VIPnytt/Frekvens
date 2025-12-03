import { mdiWeatherSunny } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'OpenWeather';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWeatherSunny}
    />
);
