import { mdiLan, mdiLaptop } from '@mdi/js';
import { Component } from 'solid-js';

import { Center } from '../components/Center';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { name as ModesName } from '../services/Modes';

export const name = 'Distributed Display Protocol';

export const Main: Component = () => (
    <Center>
        <h2 class="text-4xl">
            <Icon path={mdiLaptop} />
        </h2>
        <a href={`#/${ModesName.toLowerCase()}/${name.toLowerCase().replace(/\s+/g, '-')}`}>
            <p class="text-xl mt-2 text-gray-300">
                DDP
            </p>
            <p class="text-sm mt-2 text-gray-300">
                {location.hostname}:4048
            </p>
        </a>
    </Center>
);

export const Link: Component = () => (
    <Tooltip text="Efficient Ethernet pixel data">
        <a
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
            href={`#/${ModesName.toLowerCase()}/${name.toLowerCase().replace(/\s+/g, '-')}`}
        >
            <Icon
                class="mr-2"
                path={mdiLan}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainSecondary: Component = () => (
    <div class="space-y-3 p-5">
        <h3 class="text-4xl text-white tracking-wide">{name}</h3>
        <div class="bg-white p-6 rounded-md">
            <div class="space-y-2">
                <p class="text-sm text-gray-500">
                    <a
                        href={`${REPOSITORY}/wiki/${ModesName}#-${name.toLowerCase().replace(/\s+/g, '-')}`}
                        target="_blank"
                    >
                        <span class="font-medium text-gray-700">
                            Client:
                        </span>
                    </a>
                    &nbsp;
                    <span class="font-mono">
                        {location.hostname}:4048
                    </span>
                </p>
                <p class="text-sm text-gray-500">
                    <span class="font-medium text-gray-700">
                        Python:
                    </span>
                    &nbsp;
                    <a
                        href={`${REPOSITORY}/blob/main/extra/Python/ddpTemplate.py`}
                        target="_blank"
                    >
                        template script
                    </a>
                </p>
            </div>
        </div>
    </div >
);

export default Main;
