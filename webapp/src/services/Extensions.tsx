import { mdiBriefcase } from '@mdi/js';
import { Component, createSignal, For, Match, Switch } from 'solid-js';

import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { EXTENSION_BUILD, EXTENSION_INFRARED, EXTENSION_MESSAGE, EXTENSION_MICROPHONE, EXTENSION_MQTT, EXTENSION_OTA, EXTENSION_PHOTOCELL, EXTENSION_PLAYLIST, EXTENSION_SIGNAL } from '../config/constants';
import { MainThird as ExtensionBuildThird, Link as ExtensionBuildLink, receiver as ExtensionBuild, name as ExtensionBuildName } from '../extensions/Build';
import { MainThird as ExtensionIrThird, Link as ExtensionIrLink, Actions as ExtensionIrActions, receiver as ExtensionIr, name as ExtensionIrName } from '../extensions/Infrared';
import { MainThird as ExtensionMessageThird, Link as ExtensionMessageLink, receiver as ExtensionMessage, name as ExtensionMessageName } from '../extensions/Message';
import { MainThird as ExtensionMicThird, Link as ExtensionMicLink, Actions as ExtensionMicActions, receiver as ExtensionMic, name as ExtensionMicName } from '../extensions/Microphone';
import { MainThird as ExtensionMqttThird, Link as ExtensionMqttLink, receiver as ExtensionMqtt, name as ExtensionMqttName } from '../extensions/Mqtt';
import { MainThird as ExtensionOtaThird, Link as ExtensionOtaLink, receiver as ExtensionOta, name as ExtensionOtaName } from '../extensions/Ota';
import { MainThird as ExtensionPhotocellThird, Link as ExtensionPhotocellLink, Actions as ExtensionPhotocellActions, receiver as ExtensionPhotocell, name as ExtensionPhotocellName } from '../extensions/Photocell';
import { MainThird as ExtensionPlaylistMain, Sidebar as ExtensionPlaylistSidebar, PlaylistActive as ExtensionPlaylistActive, Secondary as ExtensionPlaylistThird, Link as ExtensionPlaylistLink, receiver as ExtensionPlaylist, name as ExtensionPlaylistName } from '../extensions/Playlist';
import { receiver as ExtensionSignal } from '../extensions/Signal';
import { name as ExtensionsName } from './Extensions';
import { Main as ModesMain, SidebarModules as ModesSidebar } from './Modes';
import { WebServerPath, SidebarSection } from './WebServer';

export const name = 'Extensions';

const [getExtensions, setExtensions] = createSignal<string[]>([]);

export const ExtensionsList = getExtensions;

export const receiver = (json: any) => {
    json[name]?.list !== undefined && setExtensions(json[name].list);
    EXTENSION_BUILD && ExtensionBuild(json);
    EXTENSION_INFRARED && ExtensionIr(json);
    EXTENSION_MESSAGE && ExtensionMessage(json);
    EXTENSION_MICROPHONE && ExtensionMic(json);
    EXTENSION_MQTT && ExtensionMqtt(json);
    EXTENSION_OTA && ExtensionOta(json);
    EXTENSION_PHOTOCELL && ExtensionPhotocell(json);
    EXTENSION_PLAYLIST && ExtensionPlaylist(json);
    EXTENSION_SIGNAL && ExtensionSignal(json);
};

export const Main: Component = () => (
    <Switch
        fallback={
            <ModesMain />
        }>
        {
            EXTENSION_PLAYLIST && (
                <Match when={ExtensionPlaylistActive()}>
                    <ExtensionPlaylistMain />
                </Match>
            )
        }
    </Switch>
);

export const Sidebar: Component = () => (
    <Switch
        fallback={
            <ModesSidebar />
        }>
        {
            EXTENSION_PLAYLIST && (
                <Match when={ExtensionPlaylistActive()}>
                    <ExtensionPlaylistSidebar />
                </Match>
            )
        }
    </Switch>
);

export const Footer: Component = () => (
    <>
        {
            EXTENSION_MESSAGE && ExtensionsList().includes(ExtensionMessageName) && (
                <ExtensionMessageLink />
            )
        }
        {
            EXTENSION_PHOTOCELL && ExtensionsList().includes(ExtensionPhotocellName) && (
                <ExtensionPhotocellLink />
            )
        }
        {
            EXTENSION_PLAYLIST && ExtensionsList().includes(ExtensionPlaylistName) && (
                <ExtensionPlaylistLink />
            )
        }
    </>
);

export const SidebarSecondary: Component = () => (
    <SidebarSection title={name}>
        <div class="space-y-2">
            {
                EXTENSION_INFRARED && ExtensionsList().includes(ExtensionIrName) && (
                    <ExtensionIrActions />
                )
            }
            {
                EXTENSION_MICROPHONE && ExtensionsList().includes(ExtensionMicName) && (
                    <ExtensionMicActions />
                )
            }
            {
                EXTENSION_PHOTOCELL && ExtensionsList().includes(ExtensionPhotocellName) && (
                    <ExtensionPhotocellActions />
                )
            }
            <Tooltip text={`All ${name.toLowerCase()}`}>
                <a
                    href={`#/${name.toLowerCase()}`}
                    class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium min-h-[48px]"
                >
                    <Icon
                        class="mr-2"
                        path={mdiBriefcase}
                    />
                    {name} &#40;{ExtensionsList().length}&#41;
                </a>
            </Tooltip>
        </div>
    </SidebarSection>
);

export const MainThird: Component = () => (
    <Switch
        fallback={
            <div class="space-y-3 p-5">
                <h3 class="text-4xl text-white tracking-wide">{name}</h3>
                <div class="bg-white p-6 rounded-md">
                    <div class="space-y-2">
                        <For each={getExtensions()}>
                            {(extension) => {
                                return (
                                    <p class="text-gray-700 hover:font-semibold">
                                        <a
                                            href={`${REPOSITORY}/wiki/${ExtensionsName}#-${extension.toLowerCase().replace(/\s+/g, '-')}`}
                                            target="_blank"
                                        >
                                            {extension}
                                        </a>
                                    </p>
                                );
                            }}
                        </For>
                    </div>
                </div>
            </div>
        }>
        {
            EXTENSION_BUILD && ExtensionsList().includes(ExtensionBuildName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionBuildName.toLowerCase()}`}>
                    <ExtensionBuildThird />
                </Match>
            )
        }
        {
            EXTENSION_INFRARED && ExtensionsList().includes(ExtensionIrName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionIrName.toLowerCase()}`}>
                    <ExtensionIrThird />
                </Match>
            )
        }
        {
            EXTENSION_MESSAGE && ExtensionsList().includes(ExtensionMessageName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMessageName.toLowerCase()}`}>
                    <ExtensionMessageThird />
                </Match>
            )
        }
        {
            EXTENSION_MICROPHONE && ExtensionsList().includes(ExtensionMicName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMicName.toLowerCase()}`}>
                    <ExtensionMicThird />
                </Match>
            )
        }
        {
            EXTENSION_MQTT && ExtensionsList().includes(ExtensionMqttName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMqttName.toLowerCase()}`}>
                    <ExtensionMqttThird />
                </Match>
            )
        }
        {
            EXTENSION_OTA && ExtensionsList().includes(ExtensionOtaName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionOtaName.toLowerCase()}`}>
                    <ExtensionOtaThird />
                </Match>
            )
        }
        {
            EXTENSION_PHOTOCELL && ExtensionsList().includes(ExtensionPhotocellName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionPhotocellName.toLowerCase()}`}>
                    <ExtensionPhotocellThird />
                </Match>
            )
        }
        {
            EXTENSION_PLAYLIST && ExtensionsList().includes(ExtensionPlaylistName) && (
                <Match when={WebServerPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionPlaylistName.toLowerCase()}`}>
                    <ExtensionPlaylistThird />
                </Match>
            )
        }
    </Switch>
);

export const SidebarThird: Component = () => (
    <SidebarSection title={name}>
        <>
            {
                EXTENSION_BUILD && ExtensionsList().includes(ExtensionBuildName) && (
                    <ExtensionBuildLink />
                )
            }
            {
                EXTENSION_INFRARED && ExtensionsList().includes(ExtensionIrName) && (
                    <ExtensionIrLink />
                )
            }
            {
                EXTENSION_MESSAGE && ExtensionsList().includes(ExtensionMessageName) && (
                    <ExtensionMessageLink />
                )
            }
            {
                EXTENSION_MICROPHONE && ExtensionsList().includes(ExtensionMicName) && (
                    <ExtensionMicLink />
                )
            }
            {
                EXTENSION_MQTT && ExtensionsList().includes(ExtensionMqttName) && (
                    <ExtensionMqttLink />
                )
            }
            {
                EXTENSION_OTA && ExtensionsList().includes(ExtensionOtaName) && (
                    <ExtensionOtaLink />
                )
            }
            {
                EXTENSION_PHOTOCELL && ExtensionsList().includes(ExtensionPhotocellName) && (
                    <ExtensionPhotocellLink />
                )
            }
            {
                EXTENSION_PLAYLIST && ExtensionsList().includes(ExtensionPlaylistName) && (
                    <ExtensionPlaylistLink />
                )
            }
        </>
    </SidebarSection>
);

export default Main;
