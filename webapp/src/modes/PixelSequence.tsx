import { mdiLedStripVariant } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Pixel sequence';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLedStripVariant}
    />
);
