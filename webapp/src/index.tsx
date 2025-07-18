import { mdiCog, mdiHome, mdiMenu } from '@mdi/js';
import { Component, createEffect, createSignal, JSX, Match, Switch } from 'solid-js';
import { render } from 'solid-js/web';

import Button from './components/Button';
import { Center } from './components/Center';
import { ToastProvider } from './components/Toast';
import { Icon } from './components/Vector';
import { MessageProvider, WebSocketState } from './extensions/WebSocket';
import { MainSecondary as DeviceSecondary, SidebarSecondary as DeviceSecondarySidebar, DeviceName } from './services/Device';
import { Main as DisplayMain, Sidebar as DisplaySidebar, SidebarSecondary as DisplaySecondarySidebar, DisplayPower } from './services/Display';
import { Main as ExtensionsMain, Sidebar as ExtensionsSidebar, Footer as ExtensionsFooter, SidebarSecondary as ExtensionsSecondarySidebar, MainThird as ExtensionsThird, SidebarThird as ExtensionsThirdSidebar, name as ExtensionsName } from './services/Extensions';
import { MainThird as ModesThird, Sidebar as ModesSidebar, SidebarSecondary as ModesSecondarySidebar, SidebarThird as ModesThirdSidebar, name as ModesName } from './services/Modes';
import { MainThird as NetworkThird, SidebarSecondary as NetworkSecondarySidebar, SidebarThird as NetworkThirdSidebar, name as NetworkName, NetworkDomain } from './services/Network';
import { WebServerPath } from './services/WebServer';

import './style.css';

const [getSidebar, setSidebar] = createSignal<boolean>(true);

export const PageSidebar = getSidebar;

createEffect(() => {
    if (!DisplayPower() && !getSidebar()) {
        setSidebar(true);
    }
});

const Page = () => (
    <div class="h-full">
        {WebServerPath() == '/' ? (
            <Primary />
        ) : (
            <Secondary />
        )}
        {(DisplayPower() || !getSidebar()) && WebSocketState() === 1 && (
            <Toggle />
        )}
    </div>
);

const Primary: Component = () => (
    <Layout
        main={
            <Switch
                fallback={
                    <ExtensionsMain />
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
                <Switch
                    fallback={
                        <ExtensionsSidebar />
                    }>
                    <Match when={!DisplayPower()}>
                        <></>
                    </Match>
                </Switch>
                <div class="mt-auto mb-3 border-t border-gray-200" />
                <ExtensionsFooter />
                <div class="mt-3 border-t border-gray-200" />
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
                }>
                <Match when={WebServerPath().startsWith(`/${NetworkName.toLowerCase()}`) || location.hostname.endsWith('.1') || location.hostname.startsWith('[fe80::')}>
                    <NetworkThird />
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
                            <DeviceSecondarySidebar />
                            <DisplaySecondarySidebar />
                            <ExtensionsSecondarySidebar />
                            <ModesSecondarySidebar />
                            <NetworkSecondarySidebar />
                        </>
                    }>
                    <Match when={WebServerPath().startsWith(`/${NetworkName.toLowerCase()}`) || location.hostname.endsWith('.1') || location.hostname.startsWith('[fe80::')}>
                        <NetworkThirdSidebar />
                    </Match>
                    <Match when={WebServerPath().startsWith(`/${ExtensionsName.toLowerCase()}`)}>
                        <ExtensionsThirdSidebar />
                    </Match>
                    <Match when={WebServerPath().startsWith(`/${ModesName.toLowerCase()}`)}>
                        <ModesThirdSidebar />
                    </Match>
                </Switch>
                <div class="mt-auto border-t border-gray-200" />
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
    <div class={`h-full ${getSidebar() && WebSocketState() === 1 ? `grid grid-cols-[320px_1fr]` : ''}`}>
        {WebSocketState() === 1 ? (
            <>
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
            </>
        ) : (
            <main class="h-full overflow-auto">
                <Center>
                    <h2 class="text-4xl">
                        Connecting...
                    </h2>
                    <p class="text-xs mt-2 text-gray-300">
                        {DeviceName()} might be offline.
                    </p>
                </Center>
            </main>
        )}
    </div>
);

const Home: Component = () => (
    <div class="pt-6">
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
            class={`transition-colors ${getSidebar() ? 'hover:bg-red-600' : 'bg-red-600 hover:bg-green-600'}`}
            onClick={() => setSidebar(!getSidebar())}
        >
            <Icon path={mdiMenu} />
        </Button>
    </div>
);

render(
    () => (
        <ToastProvider>
            <MessageProvider>
                <Page />
            </MessageProvider>
        </ToastProvider>
    ),
    document.getElementById('app') as HTMLElement,
);