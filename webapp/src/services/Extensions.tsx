import { mdiBriefcase } from '@mdi/js';
import { Component, Match, Switch } from 'solid-js';

import { Icon } from '../components/Icon';
import { EXTENSION_INFRARED, EXTENSION_MESSAGE, EXTENSION_MICROPHONE, EXTENSION_OTA, EXTENSION_PHOTOCELL, EXTENSION_PLAYLIST, EXTENSION_SIGNAL } from '../config/modules';
import { MainThird as ExtensionIrThird, Link as ExtensionIrLink, Actions as ExtensionIrActions, receiver as ExtensionIr, name as ExtensionIrName } from '../extensions/Infrared';
import { MainThird as ExtensionMessageThird, Link as ExtensionMessageLink, receiver as ExtensionMessage, name as ExtensionMessageName } from '../extensions/Message';
import { MainThird as ExtensionMicThird, Link as ExtensionMicLink, Actions as ExtensionMicActions, receiver as ExtensionMic, name as ExtensionMicName } from '../extensions/Microphone';
import { MainThird as ExtensionOtaThird, Link as ExtensionOtaLink, name as ExtensionOtaName } from '../extensions/Ota';
import { MainThird as ExtensionPhotocellThird, Link as ExtensionPhotocellLink, Actions as ExtensionPhotocellActions, receiver as ExtensionPhotocell, name as ExtensionPhotocellName } from '../extensions/Photocell';
import { MainThird as ExtensionPlaylistThird, Link as ExtensionPlaylistLink, Actions as ExtensionPlaylistActions, receiver as ExtensionPlaylist, name as ExtensionPlaylistName } from '../extensions/Playlist';
import { receiver as ExtensionSignal } from '../extensions/Signal';
import { Main as WebAppMain, WebAppPath, SidebarSectionSecondary } from '../extensions/WebApp';
import { name as ExtensionsName } from './Extensions';

export const name = 'Extensions';

export const receiver = (json: any) => {
    EXTENSION_INFRARED && ExtensionIr(json);
    EXTENSION_MESSAGE && ExtensionMessage(json);
    EXTENSION_MICROPHONE && ExtensionMic(json);
    EXTENSION_PHOTOCELL && ExtensionPhotocell(json);
    EXTENSION_PLAYLIST && ExtensionPlaylist(json);
    EXTENSION_SIGNAL && ExtensionSignal(json);
};

export const Footer: Component = () => (
    <>
        {
            (EXTENSION_MESSAGE) ? (
                <div class="hr mt-auto mb-3" />
            ) : (
                <div class="mt-auto" />
            )
        }
        {
            (EXTENSION_MESSAGE) &&
            <div class="mx-3">
                {
                    <ExtensionMessageLink />
                }
            </div>
        }
    </>
);

export const SidebarSecondary: Component = () => (
    <SidebarSectionSecondary title={name}>
        <div class="space-y-3">
            {
                EXTENSION_INFRARED && (
                    <ExtensionIrActions />
                )
            }
            {
                EXTENSION_MICROPHONE && (
                    <ExtensionMicActions />
                )
            }
            {
                EXTENSION_PHOTOCELL && (
                    <ExtensionPhotocellActions />
                )
            }
            {
                EXTENSION_PLAYLIST && (
                    <ExtensionPlaylistActions />
                )
            }
            <a
                class="link"
                href={`#/${name.toLowerCase()}`}
            >
                <Icon
                    class="mr-2"
                    path={mdiBriefcase}
                />
                More {name.toLowerCase()}...
            </a>
        </div>
    </SidebarSectionSecondary>
);

export const MainThird: Component = () => (
    <Switch
        fallback={
            <WebAppMain />
        }
    >
        {
            EXTENSION_INFRARED && (
                <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionIrName.toLowerCase()}`}>
                    <ExtensionIrThird />
                </Match>
            )
        }
        {
            EXTENSION_MESSAGE && (
                <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMessageName.toLowerCase()}`}>
                    <ExtensionMessageThird />
                </Match>
            )
        }
        {
            EXTENSION_MICROPHONE && (
                <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMicName.toLowerCase()}`}>
                    <ExtensionMicThird />
                </Match>
            )
        }
        {
            EXTENSION_OTA && (
                <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionOtaName.toLowerCase()}`}>
                    <ExtensionOtaThird />
                </Match>
            )
        }
        {
            EXTENSION_PHOTOCELL && (
                <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionPhotocellName.toLowerCase()}`}>
                    <ExtensionPhotocellThird />
                </Match>
            )
        }
        {
            EXTENSION_PLAYLIST && (
                <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionPlaylistName.toLowerCase()}`}>
                    <ExtensionPlaylistThird />
                </Match>
            )
        }
    </Switch>
);

export const SidebarThird: Component = () => (
    <SidebarSectionSecondary title={name}>
        <>
            {
                EXTENSION_INFRARED && (
                    <ExtensionIrLink />
                )
            }
            {
                EXTENSION_MESSAGE && (
                    <ExtensionMessageLink />
                )
            }
            {
                EXTENSION_MICROPHONE && (
                    <ExtensionMicLink />
                )
            }
            {
                EXTENSION_OTA && (
                    <ExtensionOtaLink />
                )
            }
            {
                EXTENSION_PHOTOCELL && (
                    <ExtensionPhotocellLink />
                )
            }
            {
                EXTENSION_PLAYLIST && (
                    <ExtensionPlaylistLink />
                )
            }
        </>
    </SidebarSectionSecondary>
);
