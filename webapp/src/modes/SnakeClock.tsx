import { Component } from 'solid-js';

import { ClockIcon } from '../components/Clock';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Snake clock';

export const Main: Component = () => (
    <ModesMainComponent
        icon={ClockIcon()}
    />
);
