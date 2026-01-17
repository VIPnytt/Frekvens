import { mdiBlindsHorizontalClosed } from '@mdi/js';
import type { Component } from 'solid-js';

import { IkeaFrekvens } from '../config/devices';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Blinds';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiBlindsHorizontalClosed}
        text={IkeaFrekvens}
    />
);
