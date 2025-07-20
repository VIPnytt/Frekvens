import { mdiPower, mdiRestart } from '@mdi/js';
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
const [getUpdate, setUpdate] = createSignal<string>(__VERSION__);
const [getVersion, setVersion] = createSignal<string>(__VERSION__);

export const DeviceModel = getModel;
export const DeviceName = getName;
export const DeviceVersion = getVersion;

export const receiver = (json: any) => {
    json[name]?.event !== undefined && event(json[name].event);
    json[name]?.model !== undefined && setModel(json[name].model);
    json[name]?.name !== undefined && setName(json[name].name);
    json[name]?.version_available !== undefined && setUpdate(json[name].version_available);
    json[name]?.version_current !== undefined && setVersion(json[name].version_current);
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

export const MainSecondary: Component = () => {
    const versionState = compareVersions(getVersion(), __VERSION__);
    return (
        <Center>
            {versionState ? (
                <div class="space-y-3 p-5">
                    <h3 class="text-4xl text-white tracking-wide">Frekvens</h3>
                    <div class="bg-white p-6 rounded-md">
                        <div class="space-y-2">
                            <a
                                href={`${REPOSITORY}/releases`}
                                target="_blank"
                            >
                                <h3 class="font-semibold text-gray-700 tracking-wide">
                                    {versionState > 0 ? 'Web interface' : 'Firmware'} update required
                                </h3>
                                <p class="text-sm text-gray-500">
                                    The device's {versionState > 0 ? 'firmware' : 'web interface'} has been updated, but the {versionState > 0 ? 'web interface' : 'firmware'} was designed for an older version.
                                    <br />
                                    This can lead to unexpected behavior, missing features, or data corruption.
                                </p>
                                <p class="text-sm text-gray-500 mt-2">
                                    Please update your device's {versionState > 0 ? 'web interface' : 'firmware'} to ensure full compatibility and access to all features.
                                </p>
                                <p class="text-sm text-gray-700 mt-2">
                                    <span class="font-medium">Firmware:</span> {getVersion()}
                                    <br />
                                    <span class="font-medium">Web interface:</span> {__VERSION__}
                                </p>
                            </a>
                        </div>
                    </div>
                </div >
            ) : (
                <>
                    <h2 class="text-4xl">
                        {getName()}
                    </h2>
                    <a
                        href={getUpdate() == getVersion() ? REPOSITORY : `${REPOSITORY}/releases`}
                        target="_blank"
                    >
                        <Tooltip text={`Frekvens ${getVersion()}`}>
                            <p class="text-xl mt-2 text-gray-300">
                                Frekvens
                            </p>
                            {getUpdate() != getVersion() && (
                                <p class="text-xs mt-2 text-gray-300">
                                    {compareVersions(getUpdate(), getVersion()) === -1 ? 'Rollback' : 'Update'} available: {getUpdate()}
                                </p>
                            )}
                        </Tooltip>
                    </a>
                </>
            )}
        </Center>
    );
};

export const SidebarSecondary: Component = () => {
    const handlePower = (power: boolean = false) => {
        if (power || confirm('Are you sure you want to power off?')) {
            ws.send(JSON.stringify({
                [name]: {
                    action: power ? 'reboot' : 'power',
                },
            }));
        }
    };

    return (
        <SidebarSection title={name}>
            <div class="space-y-2">
                <div class={`grid grid-cols-[1fr_48px_48px] gap-3 items-center font-medium text-gray-700 hover:text-gray-900`}>
                    <div>
                        <Icon
                            class="mr-2"
                            path={mdiPower}
                        />
                        Power
                    </div>
                    <Tooltip text="Reboot">
                        <Button
                            class={`w-full bg-blue-600 hover:bg-red-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded`}
                            onClick={() => handlePower(true)}
                        >
                            <Icon path={mdiRestart} />
                        </Button>
                    </Tooltip>
                    <Tooltip text="Power off">
                        <Button
                            class="w-full bg-blue-600 hover:bg-red-600 border-0 px-4 py-3 leading-6 tracking-wider cursor-pointer hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                            onClick={() => handlePower(false)}
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
