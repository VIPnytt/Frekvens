import { mdiCog, mdiHome, mdiMenu } from '@mdi/js';
import { Component, createEffect, createSignal, JSX, Match, Switch } from 'solid-js';
import { render } from 'solid-js/web';

import { Button } from './components/Button';
import { Center } from './components/Center';
import { ToastProvider } from './components/Toast';
import { Icon } from './components/Vector';
import { EXTENSION_OTA } from './config/constants';
import { MainThird as ExtensionOtaThird, OtaAuth } from './extensions/Ota';
import { MessageProvider, WebSocketState } from './extensions/WebSocket';
import { MainThird as ConnectivityThird, SidebarSecondary as ConnectivitySecondarySidebar, SidebarThird as ConnectivityThirdSidebar, name as ConnectivityName } from './services/Connectivity';
import { MainSecondary as DeviceSecondary, SidebarSecondary as DeviceSecondarySidebar, DeviceName, DeviceVersion } from './services/Device';
import { Main as DisplayMain, Sidebar as DisplaySidebar, SidebarSecondary as DisplaySecondarySidebar, DisplayPower } from './services/Display';
import { Footer as ExtensionsFooter, SidebarSecondary as ExtensionsSecondarySidebar, MainThird as ExtensionsThird, SidebarThird as ExtensionsThirdSidebar, name as ExtensionsName } from './services/Extensions';
import { Main as ModesMain, MainThird as ModesThird, Sidebar as ModesSidebar, SidebarSecondary as ModesSecondarySidebar, SidebarThird as ModesThirdSidebar, name as ModesName } from './services/Modes';
import { WebServerPath } from './services/WebServer';

import './style.css';

const [getSidebar, setSidebar] = createSignal<boolean>(true);

export const PageSidebar = getSidebar;
export const PageSidebarSet = setSidebar;

createEffect(() => {
    if (!DisplayPower() && !getSidebar()) {
        setSidebar(true);
    }
});

const Page: Component = () => (
    <>
        {
            WebServerPath() == '/' && location.hostname != '192.168.4.1' && !location.hostname.startsWith('[fe80::') ? (
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
                <ExtensionsFooter />
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
                    <DeviceSecondary />
                }
            >
                <Match when={WebServerPath().startsWith(`/${ConnectivityName.toLowerCase()}`) || location.hostname == '192.168.4.1' || location.hostname.startsWith('[fe80::')}>
                    <ConnectivityThird />
                </Match>
                <Match when={WebServerPath().startsWith(`/${ExtensionsName.toLowerCase()}`)}>
                    <ExtensionsThird />
                </Match>
                <Match when={WebServerPath().startsWith(`/${ModesName.toLowerCase()}`)}>
                    <ModesThird />
                </Match>
            </Switch>
        }
        sidebar={
            <>
                <Switch
                    fallback={
                        <>
                            <ConnectivitySecondarySidebar />
                            <DeviceSecondarySidebar />
                            <DisplaySecondarySidebar />
                            <ExtensionsSecondarySidebar />
                            <ModesSecondarySidebar />
                        </>
                    }
                >
                    <Match when={WebServerPath().startsWith(`/${ConnectivityName.toLowerCase()}`) || location.hostname == '192.168.4.1' || location.hostname.startsWith('[fe80::')}>
                        <ConnectivityThirdSidebar />
                    </Match>
                    <Match when={WebServerPath().startsWith(`/${ExtensionsName.toLowerCase()}`)}>
                        <ExtensionsThirdSidebar />
                    </Match>
                    <Match when={WebServerPath().startsWith(`/${ModesName.toLowerCase()}`)}>
                        <ModesThirdSidebar />
                    </Match>
                </Switch>
                <Home />
            </>
        }
    />
);

const Layout: Component<{
    main: JSX.Element;
    ref?: any;
    sidebar: JSX.Element;
}> = (props) => (
    <div class={`h-full ${getSidebar() ? `grid grid-cols-[320px_1fr]` : ''}`}>
        {getSidebar() && (
            <aside class="bg-white p-6 flex flex-col h-full">
                {props.sidebar}
            </aside>
        )}
        <main
            class="h-full overflow-auto"
            ref={props.ref}
        >
            <div class="grid h-full justify-center items-center">
                {props.main}
            </div>
        </main>
    </div>
);

const Home: Component = () => (
    <div class={`pt-6 border-t border-gray-200 ${WebServerPath() == '/' ? 'mt-3 ' : 'mt-auto'}`}>
        <a
            href={`#/${WebServerPath() == '/' ? 'menu' : ''}`}
            class="inline-flex items-center text-gray-700 hover:text-gray-900 font-medium"
        >
            <Icon
                class="mr-2"
                path={WebServerPath() == '/' ? mdiCog : mdiHome}
            />
            {WebServerPath() == '/' ? 'Settings' : 'Home'}
        </a>
    </div>
);

const Toggle: Component = () => (
    <div class={`absolute ${getSidebar() ? 'left-[320px] top-0' : 'left-6 top-6'}`}>
        <Button
            class={`transition-all ${getSidebar() ? 'hover:bg-red-600' : 'bg-red-600 hover:bg-green-600'}`}
            onClick={() => setSidebar(!getSidebar())}
        >
            <Icon path={mdiMenu} />
        </Button>
    </div>
);

const Incompatible: Component = () => (
    <>
        <div class="space-y-3 p-5">
            <h3 class="text-4xl text-white tracking-wide">Frekvens</h3>
            <div class="bg-white p-6 rounded-md">
                <div class="space-y-2">
                    <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                        Version conflict
                    </h3>
                    <p class="text-sm">
                        Firmware: {DeviceVersion()}<br />
                        Filesystem: {__VERSION__}
                    </p>
                    <p class="text-sm">
                        Flash or upload matching versions to restore compatibility.
                    </p>
                </div>
            </div>
            <p class="text-sm text-white tracking-wide">
                <a
                    href={REPOSITORY}
                    target="_blank"
                >
                    View project on GitHub
                </a>
            </p>
        </div>
        {
            EXTENSION_OTA && !OtaAuth() && (
                <ExtensionOtaThird />
            )
        }
    </>
);

const Offline: Component = () => (
    <Center>
        <h2 class="text-4xl">
            Connecting...
        </h2>
        <p class="text-xs mt-2 text-gray-300">
            {DeviceName()} might be offline.
        </p>
    </Center>
);

render(
    () => (
        <ToastProvider>
            <MessageProvider>
                <div class="h-full">
                    <Switch
                        fallback={
                            <Page />
                        }
                    >
                        <Match when={WebSocketState() !== 1}>
                            <Offline />
                        </Match>
                        <Match when={__VERSION__ != DeviceVersion()}>
                            <Incompatible />
                        </Match>
                    </Switch>
                </div>
            </MessageProvider>
        </ToastProvider>
    ),
    document.getElementById('app') as HTMLElement,
);