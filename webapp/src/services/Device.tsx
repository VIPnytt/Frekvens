import { mdiPower } from '@mdi/js';
import { compareVersions } from 'compare-versions';
import { Component, createEffect, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Center } from '../components/Center';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { MODEL, NAME } from '../config/constants';
import { MODEL as MODEL_FREKVENS } from '../config/Frekvens';
import { ws } from '../extensions/WebSocket';
import { SidebarSection } from './WebServer';

export const name = 'Device';

const [getModel, setModel] = createSignal<string>(MODEL || MODEL_FREKVENS);
const [getName, setName] = createSignal<string>(NAME || MODEL || MODEL_FREKVENS);
const [getVersionLatest, setVersionLatest] = createSignal<string>(__VERSION__);

export const DeviceModel = getModel;
export const DeviceName = getName;

export const receiver = (json: any) => {
    json[name]?.event !== undefined && event(json[name].event);
    json[name]?.model !== undefined && setModel(json[name].model);
    json[name]?.name !== undefined && setName(json[name].name);
    json[name]?.version_latest !== undefined && setVersionLatest(json[name].version_latest);
};

createEffect(() => {
    document.title = getName();
});

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case 'power':
            toast('Power off', 60e3);
            break;
        case 'reboot':
            toast('Rebooting', 10e3);
            break;
    }
};

export const MainSecondary = () => (
    <Center>
        <h2 class="text-4xl">
            {getName()}
        </h2>
        <a
            href={getVersionLatest() == __VERSION__ ? REPOSITORY : `${REPOSITORY}/releases`}
            target="_blank"
        >
            <Tooltip text={`Frekvens ${__VERSION__}`}>
                <p class="text-xl mt-2 text-gray-300">
                    Frekvens
                </p>
                {getVersionLatest() != __VERSION__ && (
                    <p class="text-xs mt-2 text-gray-300">
                        {compareVersions(getVersionLatest(), __VERSION__) === -1 ? 'Rollback' : 'Update'} available: {getVersionLatest()}
                    </p>
                )}
            </Tooltip>
        </a>
    </Center>
);

export const SidebarSecondary: Component = () => {
    const handlePower = () => {
        if (confirm('Are you sure you want to power off?')) {
            ws.send(JSON.stringify({
                [name]: {
                    action: 'power',
                },
            }));
        }
    };

    return (
        <SidebarSection title={name}>
            <div class="space-y-2">
                <div class={`grid grid-cols-[1fr_48px] gap-3 items-center font-medium text-gray-700 hover:text-gray-900`}>
                    <div>
                        <Icon
                            class="mr-2"
                            path={mdiPower}
                        />
                        Power
                    </div>
                    <Tooltip text="Power off">
                        <Button
                            class="w-full bg-blue-600 hover:bg-red-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                            onClick={() => handlePower()}
                        >
                            <Icon path={mdiPower} />
                        </Button>
                    </Tooltip>
                </div>
            </div>
        </SidebarSection>
    );
};

export default MainSecondary;
