import { Component, createSignal, JSX, onCleanup, onMount } from 'solid-js';

export const name = 'WebServer';

const [getHash, setHash] = createSignal(location.hash);
const onChange = () => setHash(location.hash);

onMount(() => {
    addEventListener('hashchange', onChange, false);
});

onCleanup(() => {
    removeEventListener('hashchange', onChange, false);
});

export const WebServerPath = () => getHash().replace(/^#/, '') || '/';

export const SidebarSection: Component<{
    title: string;
    children: JSX.Element;
}> = (props) => (
    <>
        <div class="space-y-3">
            <h3 class="text-sm font-semibold text-gray-700 uppercase tracking-wide">
                {props.title}
            </h3>
            <div class="space-y-2">{props.children}</div>
        </div>
        <div class="my-6 border-t border-gray-200" />
    </>
);

export default WebServerPath;
