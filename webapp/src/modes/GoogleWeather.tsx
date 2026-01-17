import { mdiGoogleCloud } from '@mdi/js';
import type { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Google Weather';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiGoogleCloud}
    />
);
