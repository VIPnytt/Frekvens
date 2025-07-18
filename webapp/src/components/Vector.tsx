import { Component, JSX } from 'solid-js';

const namespace = 'http://www.w3.org/2000/svg';
const mdiViewBox = '0 0 24 24';

export const Icon: Component<{
    class?: string;
    fill?: string;
    path: string;
    style?: string;
    transform?: string;
    viewBox?: string;
}> = (props) => {
    return props.transform ? (
        <Svg
            aria-hidden={true}
            class={`align-[-0.25em] h-[1.25em] inline-block ${props.class || ''}`}
            role="img"
            style={props.style}
            viewBox={props.viewBox || mdiViewBox}
        >
            <G transform={props.transform}>
                <Path
                    d={props.path}
                    fill={props.fill || 'currentColor'}
                />
            </G>
        </Svg>
    ) : (
        <Svg
            aria-hidden={true}
            class={`align-[-0.25em] h-[1.25em] inline-block ${props.class || ''}`}
            role="img"
            style={props.style}
            viewBox={props.viewBox || mdiViewBox}
        >
            <Path
                d={props.path}
                fill={props.fill || 'currentColor'}
            />
        </Svg>
    );
};

export function IconUri(path: string, viewBox: string = mdiViewBox): string {
    return `data:image/svg+xml,${encodeURIComponent(`<svg xmlns="${namespace}" viewBox="${viewBox}"><path d="${path}"/></svg>`)}`;
}

export function Svg(props: JSX.SvgSVGAttributes<SVGSVGElement>) {
    return (
        <svg
            xmlns={props.xmlns || namespace}
            viewBox={props.viewBox || mdiViewBox}
            {...props}
        >
            {props.children}
        </svg>
    );
}

export function G(props: JSX.GSVGAttributes<SVGGElement>) {
    return (
        <g {...props}>
            {props.children}
        </g>
    );
}

export function Path(props: JSX.PathSVGAttributes<SVGPathElement> & { d: string }) {
    return (
        <path {...props}>
        </path>
    );
}
