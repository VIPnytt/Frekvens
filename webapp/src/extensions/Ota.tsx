import { mdiProgressUpload } from '@mdi/js';
import { Component, createSignal, For } from 'solid-js';

import { Button } from '../components/Button';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { DeviceVersion } from '../services/Device';
import { name as ExtensionsName } from '../services/Extensions';


export const name = 'OTA';

const [getAuth, setAuth] = createSignal<boolean>(false);
const [getPlatformioIni, setPlatformioIni] = createSignal<Record<string, string>>({});

export const receiver = (json: any) => {
    json[name]?.['platformio.ini'] !== undefined && setPlatformioIni(json[name]['platformio.ini']);
    json[name]?.['platformio.ini']?.upload_flags !== undefined && setAuth(json[name]['platformio.ini'].upload_flags.includes('--auth'));
};

export const Link: Component = () => (
    <Tooltip text="Over The Air updates">
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiProgressUpload}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => {
    const { toast } = Toast();

    const handleFile = (e: Event & {
        currentTarget: HTMLInputElement;
        target: HTMLInputElement;
    }) => {
        const upload = document.getElementById('upload') as HTMLInputElement;
        if (e.currentTarget.files === null || e.currentTarget.files.length === 0) {
            upload.disabled = true;
            upload.textContent = 'Update';
            return;
        }
        upload.disabled = false;
        upload.textContent = e.currentTarget.files[0].name.toLowerCase().includes('spiffs') ? 'Update SPIFFS' : 'Update Firmware';
    };

    const handleUpload = () => {
        const binary = document.getElementById("binary") as HTMLInputElement;
        if (binary?.files === null || binary.files.length === 0) {
            return;
        }
        binary.disabled = true;
        const form = new FormData();
        form.append('binary', binary.files[0]);
        fetch('/api/ota', {
            method: 'POST',
            body: form,
        }).finally(() => {
            toast('Rebooting...', 10e3);
            window.location.reload();
        });
        const upload = document.getElementById('upload') as HTMLInputElement;
        upload.disabled = true;
        upload.classList.add('bg-red-600');
        upload.textContent = binary.files[0].name.toLowerCase().includes('spiffs') ? 'Updating SPIFFS...' : 'Updating Firmware...';
        toast('System update in progress...', 10e3);
    };

    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <div class="grid gap-3">
                        {
                            !getAuth() && (
                                <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                                    Automatic
                                </h3>
                            )
                        }
                        <p class="text-sm">
                            Update <span class="italic">over the air</span> by adding the following lines to your&nbsp;
                            <a
                                class="italic"
                                href={`${REPOSITORY}/blob/main/platformio.ini`}
                                target="_blank"
                            >
                                platformio.ini
                            </a>
                            &nbsp;file:
                        </p>
                        <p class="font-mono text-sm text-gray-700 whitespace-nowrap">
                            <For each={Object.entries(getPlatformioIni()).sort()}>
                                {([key, value]) => {
                                    return (
                                        <>
                                            <a
                                                href={`https://github.com/search?q=${encodeURIComponent(`repo:${URL.parse(REPOSITORY)?.pathname.slice(1)}`)}+${encodeURIComponent(`/\\b${key}\\b/`)}`}
                                                target="_blank"
                                            >
                                                <span class="lowercase">
                                                    {key}
                                                </span>
                                                &nbsp;
                                                <span class="text-gray-300">
                                                    =
                                                </span>
                                                &nbsp;
                                                <span class="text-gray-500">
                                                    {key == 'upload_port' ? location.hostname : value}
                                                </span>
                                            </a>
                                            <br />
                                        </>
                                    );
                                }}
                            </For>
                        </p>
                        {
                            !getAuth() && (
                                <>
                                    <div class="mt-auto border-t border-gray-200" />
                                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                                        Manual
                                    </h3>
                                    <p class="text-sm">
                                        Got a&nbsp;
                                        <a
                                            class="italic"
                                            href={`${REPOSITORY}/wiki/${ExtensionsName}#${name.toLocaleLowerCase()}`}
                                            target="_blank"
                                        >
                                            firmware.bin
                                        </a>
                                        &nbsp;or&nbsp;
                                        <a
                                            class="italic"
                                            href={`${REPOSITORY}/wiki/${ExtensionsName}#${name.toLocaleLowerCase()}`}
                                            target="_blank"
                                        >
                                            spiffs.bin
                                        </a>
                                        &nbsp;file? Upload it here:
                                    </p>
                                    <input
                                        class="w-full px-3 py-2 bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                                        id="binary"
                                        type="file"
                                        accept=".bin"
                                        onchange={handleFile}
                                    />
                                    <Button
                                        class="p-2 hover:bg-red-600 rounded-lg transition-all duration-200"
                                        id="upload"
                                        disabled
                                        onClick={handleUpload}
                                    >
                                        Update
                                    </Button>
                                    <p class="text-center text-sm text-gray-300">
                                        Firmware v{DeviceVersion()} — Filesystem v{__VERSION__}
                                    </p>
                                </>
                            )
                        }
                    </div>
                </div>
            </div>
        </div >
    );
};

export default MainThird;
