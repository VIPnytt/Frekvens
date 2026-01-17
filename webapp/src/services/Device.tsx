import { mdiPower } from '@mdi/js';
import { type Component, createEffect, createSignal } from 'solid-js';

import { Icon } from '../components/Icon';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { NAME } from '../config/constants';
import { VERSION } from '../config/version';
import { WebSocketWS } from '../extensions/WebSocket';
import { DisplayPowerSet } from './Display';

export const name = 'Device';

const [getVersion, setVersion] = createSignal<string>(VERSION);

export const DeviceVersion = getVersion;

export const receiver = (json: any) => {
    json[name]?.event !== undefined && event(json[name].event);
    json[name]?.version !== undefined && setVersion(json[name].version);
};

createEffect(() => {
    document.title = NAME;
});

const { toast } = Toast();

const event = (action: string) => {
    switch (action) {
        case 'power':
            DisplayPowerSet(false);
            toast('Power off', 60e3);
            break;
        case 'reboot':
            DisplayPowerSet(false);
            toast('Rebooting', 10e3);
            break;
    }
};

export const SidebarSecondaryComponent: Component = () => {
    const handlePower = () => {
        if (confirm('Are you sure you want to power off?')) {
            DisplayPowerSet(false);
            WebSocketWS.send(JSON.stringify({
                [name]: {
                    action: 'power',
                },
            }));
        }
    };

    return (
        <div class="action">
            <div>
                <Icon
                    class="mr-2"
                    path={mdiPower}
                />
                Power
            </div>
            <Tooltip text="Power off">
                <button
                    class="bg-positive-light dark:bg-positive-dark hover:bg-negative-alt-light dark:hover:bg-negative-alt-dark text-interactive-light dark:text-content-dark w-full"
                    onclick={() => handlePower()}
                    type="button"
                >
                    <Icon path={mdiPower} />
                </button>
            </Tooltip>
        </div>
    );
};
