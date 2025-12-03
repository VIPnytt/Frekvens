import { mdiLeaf } from '@mdi/js';
import { Component } from 'solid-js';

import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Leaf fall';

export const Main: Component = () => (
    <ModesMainComponent
        icon={mdiLeaf}
    />
);
