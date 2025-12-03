import { Component, JSX } from 'solid-js';

export const Tooltip: Component<{
    text: string;
    children: JSX.Element;
}> = (props) => (
    <div class="group inline-block relative w-full">
        {props.children}
        <div class="absolute bg-base-light dark:bg-base-dark group-hover:opacity-100 left-0 min-w-max px-2 py-1 rounded opacity-0 pointer-events-none text-sm -top-8 w-max z-50">
            {props.text}
            <div class="absolute border-t-1.5 border-x-1.5 border-x-transparent -bottom-1.5 left-4" />
        </div>
    </div>
);
