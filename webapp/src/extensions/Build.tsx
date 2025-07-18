import { mdiPackageVariant } from '@mdi/js';
import { Component, createSignal, For } from 'solid-js';

import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { name as ExtensionsName } from '../services/Extensions';

export const name = 'Build';

const [getDotEnv, setDotEnv] = createSignal<Record<string, boolean | null | number | string>>({});
const [getPlatformioIni, setPlatformioIni] = createSignal<Record<string, number | string>>({});
const [getSecretsH, setSecretsH] = createSignal<Record<string, boolean | null | number | string>>({});

export const receiver = (json: any) => {
    json[name]?.['.env'] !== undefined && setDotEnv(json[name]['.env']);
    json[name]?.['platformio.ini'] !== undefined && setPlatformioIni(json[name]['platformio.ini']);
    json[name]?.['secrets.h'] !== undefined && setSecretsH(json[name]['secrets.h']);
};

export const Link: Component = () => (
    <Tooltip text="Build and compile options">
        <a
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
        >
            <Icon
                class="mr-2"
                path={mdiPackageVariant}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => {
    const board = getPlatformioIni()['board'].toString() ?? 'custom';
    const search = `https://github.com/search?q=${encodeURIComponent(`repo:${URL.parse(REPOSITORY)?.pathname.slice(1)}`)}`;
    return (
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">{name}</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <div class="grid gap-3">
                        <a
                            class="hover:font-semibold"
                            href={`${REPOSITORY}/blob/main/platformio.ini`}
                            target="_blank"
                        >
                            <h3 class="text-sm font-semibold text-gray-700 tracking-wide">PlatformIO <span class="text-gray-300">/platformio.ini</span></h3>
                        </a>
                        <p class="font-mono text-sm text-gray-700 whitespace-nowrap">
                            <a
                                class="hover:font-semibold"
                                href={`${search}+${encodeURIComponent('/^\\[env:.*\\]$/')}`}
                                target="_blank"
                            >
                                <span class="text-gray-300">
                                    [env:<span class="text-gray-700 lowercase">{board.toLowerCase()}</span>]
                                </span>
                            </a>
                            <br />
                            <For each={Object.entries(getPlatformioIni()).sort()}>
                                {([key, value]) => {
                                    return (
                                        <>
                                            <a
                                                class="hover:font-semibold"
                                                href={key == 'board' ? `https://docs.platformio.org/en/stable/boards/espressif32/${value.toString()}.html` : `${search}+${encodeURIComponent(`/\\b${key.split('.').at(-1)}\\b/`)}`}
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
                                                    {key == 'upload_port' ? location.hostname : value.toLocaleString()}
                                                </span>
                                            </a>
                                            <br />
                                        </>
                                    );
                                }}
                            </For>
                        </p>
                        <div class="mt-auto border-t border-gray-200" />
                        <a
                            class="hover:font-semibold"
                            href={`${REPOSITORY}/blob/main/firmware/config/secrets.h`}
                            target="_blank"
                        >
                            <h3 class="text-sm font-semibold text-gray-700 tracking-wide">C++ <span class="text-gray-300">/firmware/config/secrets.h</span></h3>
                        </a>
                        <p class="font-mono text-sm text-gray-700 whitespace-nowrap">
                            <For each={Object.entries(getSecretsH()).sort()}>
                                {([key, value]) => {
                                    return (
                                        <>
                                            <a
                                                class="hover:font-semibold"
                                                href={`${search}+${encodeURIComponent(`/\\b${key}\\b/`)}`}
                                                target="_blank"
                                            >
                                                <span class="text-gray-300">
                                                    #define
                                                </span>
                                                &nbsp;
                                                <span class="uppercase">
                                                    {key}
                                                </span>
                                                &nbsp;
                                                {
                                                    typeof value === 'string' ? (
                                                        <span class="text-gray-300">"<span class="text-gray-500">{value}</span>"</span>
                                                    ) : (
                                                        <span class="text-gray-500">{value?.toLocaleString()}</span>
                                                    )
                                                }
                                            </a>
                                            <br />
                                        </>
                                    );
                                }}
                            </For>
                        </p>
                        <div class="mt-auto border-t border-gray-200" />
                        <a
                            class="hover:font-semibold"
                            href={`${REPOSITORY}/blob/main/.env`}
                            target="_blank"
                        >
                            <h3 class="text-sm font-semibold text-gray-700 tracking-wide">Environment <span class="text-gray-300">/.env</span></h3>
                        </a>
                        <p class="font-mono text-sm text-gray-700 whitespace-nowrap">
                            <For each={Object.entries(getDotEnv()).sort()}>
                                {([key, value]) => {
                                    return (
                                        <>
                                            <a
                                                class="hover:font-semibold"
                                                href={`${search}+${encodeURIComponent(`/\\b(VITE_)?${key}\\b/`)}`}
                                                target="_blank"
                                            >
                                                <span class="uppercase">
                                                    {key}
                                                </span>
                                                <span class="text-gray-300">
                                                    ='<span class="text-gray-500">{value?.toLocaleString()}</span>'
                                                </span>
                                            </a>
                                            <br />
                                        </>
                                    );
                                }}
                            </For>
                        </p>
                    </div>
                </div>
            </div>
        </div >
    );
};

export default MainThird;
