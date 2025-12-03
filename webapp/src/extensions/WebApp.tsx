import { mdiDockLeft, mdiDotsGrid, mdiHome, mdiMenu, mdiWindowClose } from '@mdi/js';
import { Component, createEffect, createSignal, JSX, Match, onCleanup, onMount, Switch } from 'solid-js';

import { Icon } from '../components/Icon';
import { Tooltip } from '../components/Tooltip';
import { ToastProvider } from '../components/Toast';
import { HOSTNAME, NAME } from '../config/constants';
import { Device } from '../config/devices';
import { EXTENSION_OTA } from '../config/modules';
import { VERSION } from '../config/version';
import { MainComponent as ExtensionOtaComponent } from './Ota';
import { WebSocketWS, WebSocketmessages, WebSocketState } from './WebSocket';
import { MainThird as ConnectivityThird, SidebarSecondaryComponent as ConnectivitySecondarySidebarComponent, SidebarThird as ConnectivityThirdSidebar, name as ConnectivityName } from '../services/Connectivity';
import { SidebarSecondaryComponent as DeviceSecondarySidebarComponent, DeviceVersion } from '../services/Device';
import { Main as DisplayMain, Sidebar as DisplaySidebar, SidebarSecondaryComponent as DisplaySecondarySidebarComponent, DisplayPower } from '../services/Display';
import { Footer as ExtensionsFooter, SidebarSecondary as ExtensionsSecondarySidebar, MainThird as ExtensionsThird, SidebarThird as ExtensionsThirdSidebar, name as ExtensionsName } from '../services/Extensions';
import { Main as ModesMain, MainThird as ModesThird, Sidebar as ModesSidebar, SidebarSecondary as ModesSecondarySidebar, SidebarThird as ModesThirdSidebar, name as ModesName } from '../services/Modes';

export const name = 'Web app';
export const WebAppUrl = `http://${HOSTNAME}.local`;

const [getHash, setHash] = createSignal(location.hash);
const [getSidebar, setSidebar] = createSignal<boolean>(true);

export const WebAppSidebar = getSidebar;
export const WebAppSidebarSet = setSidebar;

const onChange = () => setHash(location.hash);

createEffect(() => {
    if (!DisplayPower() && !getSidebar()) {
        setSidebar(true);
    }
});

onMount(() => {
    addEventListener('hashchange', onChange, false);
});

onCleanup(() => {
    removeEventListener('hashchange', onChange, false);
});

export const WebAppPath = () => getHash().replace(/^#/, '') || '/';

export const WebApp: Component = () => (
    <WebSocketmessages>
        <ToastProvider>
            <Switch
                fallback={
                    <>
                        {
                            WebAppPath() == '/' ? (
                                <Primary />
                            ) : (
                                <Secondary />
                            )
                        }
                        {
                            (DisplayPower() || !getSidebar()) && (
                                <Toggle />
                            )
                        }
                    </>
                }
            >
                <Match when={WebSocketState() !== WebSocketWS.OPEN}>
                    <Main />
                </Match>
                <Match when={VERSION !== DeviceVersion()}>
                    <Third />
                </Match>
            </Switch>
        </ToastProvider>
    </WebSocketmessages>

);

const Primary: Component = () => (
    <Layout
        main={
            <Switch
                fallback={
                    <ModesMain />
                }
            >
                <Match when={!DisplayPower()}>
                    <DisplayMain />
                </Match>
            </Switch>
        }
        sidebar={
            <>
                <DisplaySidebar />
                <ModesSidebar />
                <Home />
            </>
        }
    />
);

const Secondary: Component = () => (
    <Layout
        main={
            <Switch
                fallback={
                    <Main />
                }
            >
                <Match when={WebAppPath().startsWith(`/${ConnectivityName.toLowerCase()}`)}>
                    <ConnectivityThird />
                </Match>
                <Match when={WebAppPath().startsWith(`/${ExtensionsName.toLowerCase()}`)}>
                    <ExtensionsThird />
                </Match>
                <Match when={WebAppPath().startsWith(`/${ModesName.toLowerCase()}`)}>
                    <ModesThird />
                </Match>
            </Switch>
        }
        sidebar={
            <>
                <Switch
                    fallback={
                        <>
                            <SidebarSectionSecondary title={Device.MODEL}>
                                <div class="space-y-3">
                                    <ConnectivitySecondarySidebarComponent />
                                    <DeviceSecondarySidebarComponent />
                                    <DisplaySecondarySidebarComponent />
                                </div>
                            </SidebarSectionSecondary>
                            <ExtensionsSecondarySidebar />
                            <ModesSecondarySidebar />
                        </>
                    }
                >
                    <Match when={WebAppPath().startsWith(`/${ConnectivityName.toLowerCase()}`)}>
                        <ConnectivityThirdSidebar />
                    </Match>
                    <Match when={WebAppPath().startsWith(`/${ExtensionsName.toLowerCase()}`)}>
                        <ExtensionsThirdSidebar />
                    </Match>
                    <Match when={WebAppPath().startsWith(`/${ModesName.toLowerCase()}`)}>
                        <ModesThirdSidebar />
                    </Match>
                </Switch>
                <Home />
            </>
        }
    />
);

export const Third: Component = () => (
    <div class="main">
        <div class="space-y-3 p-5">
            <h2>
                Frekvens
            </h2>
            <div class="box">
                <div class="space-y-3">
                    <h3>
                        Version conflict
                    </h3>
                    <div class="text-sm">
                        Firmware: {DeviceVersion()}<br />
                        Filesystem: {VERSION}
                    </div>
                    <div class="text-sm">
                        Flash or upload matching versions to restore compatibility.
                    </div>
                </div>
            </div>
            <div class="text-sm tracking-wide">
                <a
                    href="https://github.com/VIPnytt/Frekvens"
                    target="_blank"
                >
                    Download the latest version
                </a>
            </div>
        </div>
        {
            EXTENSION_OTA && (
                <ExtensionOtaComponent />
            )
        }
    </div>
);

export const Main: Component = () => (
    <div class="main">
        <h1>
            <Icon
                path={mdiDotsGrid}
            />
        </h1>
        <div class="mode-title">
            {NAME}
        </div>
    </div>
);

const Home: Component = () => (
    <>
        {
            WebAppPath() === '/' ? (
                <ExtensionsFooter />
            ) : (
                <Footer />
            )
        }
        <div class="hr mt-3 pt-3">
            <a
                class="link"
                href={`#/${WebAppPath() === '/' ? 'menu' : ''}`}
            >
                <Icon
                    class="mr-2"
                    path={WebAppPath() == '/' ? mdiMenu : mdiHome}
                />
                {WebAppPath() == '/' ? 'Menu' : 'Home'}
            </a>
        </div>
    </>
);

const Footer: Component = () => (
    <>
        <div class="hr mb-3 mt-auto" />
        <div class="mx-3">
            <div class="link">
                <a
                    class="flex-1"
                    href="https://github.com/VIPnytt/Frekvens/wiki"
                    target="_blank"
                >
                    <Icon
                        class="mr-2"
                        path={mdiDotsGrid}
                    />
                    Frekvens
                </a>
                <a
                    class="text-content-alt-light dark:text-content-alt-dark flex-none"
                    href="https://github.com/VIPnytt/Frekvens/releases"
                    target="_blank"
                >
                    <Tooltip text="Release notes">
                        {VERSION}
                    </Tooltip>
                </a>
            </div>
        </div>
    </>
);

const Toggle: Component = () => (
    <div class={`absolute ${getSidebar() ? 'left-[calc(--spacing(68)-1px)] top-0' : 'left-6 top-6'}`}>
        <button
            class={`text-interactive-light dark:text-content-dark ${getSidebar() ? 'border-0 bg-menu-light dark:bg-menu-dark hover:bg-negative-alt-light dark:hover:bg-negative-alt-dark' : 'bg-neutral-light dark:bg-neutral-dark hover:bg-positive-alt-light dark:hover:bg-positive-alt-dark'}`}
            onclick={() => setSidebar(!getSidebar())}
        >
            <Icon path={getSidebar() ? mdiWindowClose : mdiDockLeft} />
        </button>
    </div>
);

export const SidebarSection: Component<{
    title: string;
    children: JSX.Element;
}> = (props) => (
    <>
        <div class="space-y-3">
            <h3 class="ml-3">
                {props.title}
            </h3>
            <div class="mx-3 space-y-3">
                <div class="px-3">
                    {props.children}
                </div>
            </div>
        </div>
        <div class="hr my-6" />
    </>
);

export const SidebarSectionSecondary: Component<{
    title: string;
    children: JSX.Element;
}> = (props) => (
    <>
        <div class="space-y-3">
            <h3 class="ml-3">
                {props.title}
            </h3>
            <div class="mx-3 space-y-3">
                {props.children}
            </div>
        </div>
        <div class="hr my-6" />
    </>
);

const Layout: Component<{
    main: JSX.Element;
    sidebar: JSX.Element;
}> = (props) => (
    <div class={`h-full ${getSidebar() ? `grid grid-cols-[--spacing(80)_1fr]` : ''}`}>
        {getSidebar() && (
            <aside class="bg-menu-light dark:bg-menu-dark border-r flex flex-col h-full p-2 pt-4 rounded-none">
                {props.sidebar}
            </aside>
        )}
        <main class="h-full overflow-auto rounded-none">
            {props.main}
        </main>
    </div>
);

export const Canonical = () => {
    import.meta.env.PROD && `${HOSTNAME}.local` !== location.hostname && fetch(WebAppUrl, {
        method: 'HEAD',
    }).then((response) => {
        if (response.status === 200) {
            WebSocketWS.close();
            location.href = WebAppUrl;
        }
    })
};
