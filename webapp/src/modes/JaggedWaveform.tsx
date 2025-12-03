import { mdiWaveform } from '@mdi/js';
import { Component } from 'solid-js';

import { IkeaFrekvens } from '../config/devices';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Jagged waveform';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiWaveform}
        text={IkeaFrekvens}
    />
);
