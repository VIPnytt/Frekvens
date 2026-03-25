import type { Component } from "solid-js";

export const Icon: Component<{
    class?: string;
    path: string;
    fill?: string;
    viewBox?: string;
    xmlns?: string;
}> = (props) => (
    <svg
        aria-hidden={true}
        class={`align-[-0.25em] h-[1.25em] inline-block ${props.class ?? ""}`}
        role="img"
        viewBox={props.viewBox ?? "0 0 24 24"}
        xmlns={props.xmlns ?? "http://www.w3.org/2000/svg"}
    >
        <path
            d={props.path}
            fill={props.fill ?? "currentColor"}
        />
    </svg>
);
