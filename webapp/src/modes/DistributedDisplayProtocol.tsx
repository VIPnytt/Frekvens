import { mdiLaptop } from '@mdi/js';
import { Component } from 'solid-js';

import { Center } from '../components/Center';
import { Icon } from '../components/Vector';

export const name = 'Distributed Display Protocol';

export const Main: Component = () => (
    <Center>
        <h2 class="text-4xl">
            <Icon path={mdiLaptop} />
        </h2>
        <p class="text-xl mt-2 text-gray-300">
            {name}
        </p>
        <p class="text-sm mt-2 text-gray-300">
            {location.hostname}:4048
        </p>
    </Center>
);

export default Main;
