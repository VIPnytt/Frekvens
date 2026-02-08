import { mdiBriefcase } from "@mdi/js";
import { type Component, Match, Switch } from "solid-js";

import { Icon } from "../components/Icon";
import {
    EXTENSION_INFRARED,
    EXTENSION_MESSAGE,
    EXTENSION_MICROPHONE,
    EXTENSION_OTA,
    EXTENSION_PHOTOCELL,
    EXTENSION_PLAYLIST,
} from "../config/modules";
import {
    Actions as ExtensionIrActions,
    Link as ExtensionIrLink,
    name as ExtensionIrName,
    MainThird as ExtensionIrThird,
} from "../extensions/Infrared";
import {
    Link as ExtensionMessageLink,
    name as ExtensionMessageName,
    MainThird as ExtensionMessageThird,
} from "../extensions/Message";
import {
    Actions as ExtensionMicActions,
    Link as ExtensionMicLink,
    name as ExtensionMicName,
    MainThird as ExtensionMicThird,
} from "../extensions/Microphone";
import { Link as ExtensionOtaLink, name as ExtensionOtaName, MainThird as ExtensionOtaThird } from "../extensions/Ota";
import {
    Actions as ExtensionPhotocellActions,
    Link as ExtensionPhotocellLink,
    name as ExtensionPhotocellName,
    MainThird as ExtensionPhotocellThird,
} from "../extensions/Photocell";
import {
    Actions as ExtensionPlaylistActions,
    Link as ExtensionPlaylistLink,
    name as ExtensionPlaylistName,
    MainThird as ExtensionPlaylistThird,
} from "../extensions/Playlist";
import { SidebarSectionSecondary, Main as WebAppMain, WebAppPath } from "../extensions/WebApp";
import { name as ExtensionsName } from "./Extensions";

export const name = "Extensions";

export const Footer: Component = () => (
    <>
        {EXTENSION_MESSAGE ? <div class="hr mt-auto mb-3" /> : <div class="mt-auto" />}
        {EXTENSION_MESSAGE && <div class="mx-3">{<ExtensionMessageLink />}</div>}
    </>
);

export const SidebarSecondary: Component = () => (
    <SidebarSectionSecondary title={name}>
        <div class="space-y-3">
            {EXTENSION_INFRARED && <ExtensionIrActions />}
            {EXTENSION_MICROPHONE && <ExtensionMicActions />}
            {EXTENSION_PHOTOCELL && <ExtensionPhotocellActions />}
            {EXTENSION_PLAYLIST && <ExtensionPlaylistActions />}
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
    <Switch fallback={<WebAppMain />}>
        {EXTENSION_INFRARED && (
            <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionIrName.toLowerCase()}`}>
                <ExtensionIrThird />
            </Match>
        )}
        {EXTENSION_MESSAGE && (
            <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMessageName.toLowerCase()}`}>
                <ExtensionMessageThird />
            </Match>
        )}
        {EXTENSION_MICROPHONE && (
            <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionMicName.toLowerCase()}`}>
                <ExtensionMicThird />
            </Match>
        )}
        {EXTENSION_OTA && (
            <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionOtaName.toLowerCase()}`}>
                <ExtensionOtaThird />
            </Match>
        )}
        {EXTENSION_PHOTOCELL && (
            <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionPhotocellName.toLowerCase()}`}>
                <ExtensionPhotocellThird />
            </Match>
        )}
        {EXTENSION_PLAYLIST && (
            <Match when={WebAppPath() === `/${ExtensionsName.toLowerCase()}/${ExtensionPlaylistName.toLowerCase()}`}>
                <ExtensionPlaylistThird />
            </Match>
        )}
    </Switch>
);

export const SidebarThird: Component = () => (
    <SidebarSectionSecondary title={name}>
        {EXTENSION_INFRARED && <ExtensionIrLink />}
        {EXTENSION_MESSAGE && <ExtensionMessageLink />}
        {EXTENSION_MICROPHONE && <ExtensionMicLink />}
        {EXTENSION_OTA && <ExtensionOtaLink />}
        {EXTENSION_PHOTOCELL && <ExtensionPhotocellLink />}
        {EXTENSION_PLAYLIST && <ExtensionPlaylistLink />}
    </SidebarSectionSecondary>
);
