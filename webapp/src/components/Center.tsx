import { Component, ParentProps } from 'solid-js';

export const Center: Component<ParentProps> = (props) => (
    <div class="grid p-8 h-full justify-center items-center">
        <div class="text-center text-white h-full flex justify-center items-center">
            <div>
                {props.children}
            </div>
        </div>
    </div>
);
