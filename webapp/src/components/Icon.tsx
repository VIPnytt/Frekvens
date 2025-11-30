import { Component } from 'solid-js';

const fill = 'currentColor';
const viewBox = '0 0 24 24';
const xmlns = 'http://www.w3.org/2000/svg';

export const Icon: Component<{
    class?: string;
    path: string;
    fill?: string
    viewBox?: string;
    xmlns?: string
}> = (props) => (
    <svg
        aria-hidden={true}
        class={`align-[-0.25em] h-[1.25em] inline-block ${props.class || ''}`}
        role="img"
        viewBox={props.viewBox || viewBox}
        xmlns={props.xmlns || xmlns}
    >
        <path
            d={props.path}
            fill={props.fill || fill}
        />
    </svg>
);

export const IconUri = (props: {
    fill?: string;
    path: string;
    viewBox?: string;
    xmlns?: string
}): string => `data:image/svg+xml,${encodeURIComponent(`<svg xmlns="${props.xmlns || xmlns}" viewBox="${props.viewBox || viewBox}" fill="${props.fill || fill}"><path d="${props.path}"/>${props.fill ? '' : '<style>:root{color:white}@media(prefers-color-scheme:light){:root{color:black}}</style>'}</svg>`)}`;
