import { Component, createEffect, createSignal, onCleanup, onMount } from 'solid-js';
import { createVisibilityObserver } from '@solid-primitives/intersection-observer';

import { Tooltip } from './Tooltip';
import { MODEL } from '../config/constants';
import { MODEL as MODEL_FREKVENS } from '../config/ikeaFrekvens';
import { MODEL as MODEL_OBEGRANSAD } from '../config/ikeaObegransad';
import { DeviceModel } from '../services/Device';
import { DisplayBrightness, DisplayColumns, DisplayRatio, DisplayPixelRatio, DisplayRows } from '../services/Display';
import { PageSidebar } from '../index';

const [getBrush, setBruh] = createSignal<number>(Math.pow(2, 8) - 1);

export const Brush: Component = () => (
    <Tooltip text={`Brush ${Math.ceil(getBrush() / (Math.pow(2, 8) - 1) * 100)} %`}>
        <input
            class="w-full my-1"
            max={Math.pow(2, 8) - 1}
            min="1"
            onInput={(e) =>
                setBruh(parseFloat(e.currentTarget.value))
            }
            type="range"
            value={getBrush()}
        />
    </Tooltip>
);

export const Canvas: Component<{
    disabled?: boolean;
    pixels: number[];
    onFrame?: (data: number[]) => void;
    onPixel?: (x: number, y: number, value: number) => void;
}> = (props) => {
    let canvasRef: HTMLCanvasElement | undefined;
    let containerRef: HTMLDivElement | undefined;
    let brushBrightness: number;
    let isDrawing: boolean = false;

    const useVisibilityObserver = createVisibilityObserver({ threshold: 0.9 });
    const visible = useVisibilityObserver(() => containerRef);

    onMount(() => {
        canvasRef?.addEventListener('pointerdown', handleDown);
        canvasRef?.addEventListener('pointerleave', handleUp);
        canvasRef?.addEventListener('pointermove', handleMove);
        canvasRef?.addEventListener('pointerup', handleUp);
        if (containerRef) {
            const resizeObserver = new ResizeObserver(() => {
                draw();
            });
            resizeObserver.observe(containerRef);

            onCleanup(() => {
                resizeObserver.disconnect();
            });
        }
        draw();
    });

    onCleanup(() => {
        canvasRef?.removeEventListener('pointerdown', handleDown);
        canvasRef?.removeEventListener('pointerleave', handleUp);
        canvasRef?.removeEventListener('pointermove', handleMove);
        canvasRef?.removeEventListener('pointerup', handleUp);
    });

    createEffect(() => {
        draw();
    });

    const draw = () => {
        if (!canvasRef || !containerRef) {
            return;
        }
        canvasRef.width = containerRef.clientWidth;
        canvasRef.height = containerRef.clientHeight;

        const pixelWidth = canvasRef.width / DisplayColumns();
        const pixelHeight = canvasRef.height / DisplayRows();
        const pixelSize = Math.min(pixelWidth, pixelHeight) - Math.min(pixelWidth, pixelHeight) * (1 - DisplayPixelRatio());
        const ctx = canvasRef.getContext('2d')!;

        const shapeEllipse = () => {
            const offsetX = (pixelWidth - pixelSize) / 2;
            const offsetY = (pixelHeight - pixelSize) / 2;
            for (let x = 0; x < DisplayColumns(); ++x) {
                for (let y = 0; y < DisplayRows(); ++y) {
                    ctx.fillStyle = toColor(props.pixels[x + y * DisplayColumns()]);
                    ctx.beginPath();
                    ctx.arc(
                        x * pixelWidth + offsetX + pixelSize / 2,
                        y * pixelHeight + offsetY + pixelSize / 2,
                        pixelSize / 2,
                        0,
                        2 * Math.PI
                    );
                    ctx.fill();
                }
            }
        }

        const shapeRectangle = () => {
            const offsetX = (pixelWidth - pixelSize) / 2;
            const offsetY = (pixelHeight - pixelSize) / 2;
            for (let x = 0; x < DisplayColumns(); ++x) {
                for (let y = 0; y < DisplayRows(); ++y) {
                    ctx.fillStyle = toColor(props.pixels[x + y * DisplayColumns()]);
                    ctx.fillRect(
                        x * pixelWidth + offsetX,
                        y * pixelHeight + offsetY,
                        pixelSize,
                        pixelSize
                    );
                }
            }
        }

        ctx.clearRect(0, 0, canvasRef.width, canvasRef.height);
        ctx.save();

        ctx.globalAlpha = 1;
        ctx.fillStyle = 'black';
        ctx.fillRect(0, 0, canvasRef.width, canvasRef.height);

        ctx.globalAlpha = (visible() ? 0.9 : 0.5) * DisplayBrightness() / (Math.pow(2, 8) - 1) + 0.1;

        if (MODEL === MODEL_OBEGRANSAD || (!MODEL && DeviceModel() === MODEL_OBEGRANSAD)) {
            shapeRectangle();
        } else {
            shapeEllipse();
        }
        ctx.restore();
    };

    const toColor = (brightness: number) => {
        let base: number = 0;
        if (MODEL === MODEL_FREKVENS || (!MODEL && DeviceModel() === MODEL_FREKVENS)) {
            base = 0.27 * (Math.pow(2, 8) - 1);
        } else if (MODEL === MODEL_OBEGRANSAD || (!MODEL && DeviceModel() === MODEL_OBEGRANSAD)) {
            base = 0.14 * (Math.pow(2, 8) - 1);
        }
        const value = brightness === 0 ? 1 : (brightness - 1) / (Math.pow(2, 8) - 2) * (Math.pow(2, 8) - 2 - base) + 1 + base;
        return `rgb(${value},${value},${value})`;
    };

    const handleEvent = (e: PointerEvent) => {
        if (!canvasRef || props.disabled) {
            return null;
        }

        const rect = canvasRef.getBoundingClientRect();
        const x = Math.floor(((e.clientX - rect.left) * canvasRef.width / rect.width) / (canvasRef.width / DisplayColumns()));
        const y = Math.floor(((e.clientY - rect.top) * canvasRef.height / rect.height) / (canvasRef.height / DisplayRows()));

        if (x >= 0 && x < DisplayColumns() && y >= 0 && y < DisplayRows()) {
            return { x, y };
        }
        return null;
    };

    const handleDown = (e: PointerEvent) => {
        if (props.disabled) {
            return;
        }
        e.preventDefault();
        const position = handleEvent(e);
        if (!position) {
            return;
        }
        isDrawing = true;
        const i = position.x + position.y * DisplayColumns();
        brushBrightness = props.pixels[i] === 0 ? getBrush() : 0;
        props.onPixel?.(position.x, position.y, brushBrightness);

        const newState = props.pixels.map((value, index) =>
            index === i ? brushBrightness : value,
        );
        props.onFrame?.(newState);
    };

    const handleMove = (e: PointerEvent) => {
        if (!isDrawing || props.disabled) {
            return;
        }
        e.preventDefault();

        const position = handleEvent(e);
        if (!position) {
            return;
        }
        const i = position.x + position.y * DisplayColumns();
        if (props.pixels[i] !== brushBrightness) {
            props.onPixel?.(position.x, position.y, brushBrightness);

            const newState = props.pixels.map((value, index) =>
                index === i ? brushBrightness : value,
            );
            props.onFrame?.(newState);
        }
    };

    const handleUp = () => {
        if (isDrawing && props.onFrame) {
            props.onFrame(props.pixels);
        }
        isDrawing = false;
    };

    return (
        <div
            class={`w-full p-4 bg-black shadow-lg relative mx-auto flex-none flex-shrink-0 inline-block transition-opacity duration-300 max-h-[calc((100vh-128px)*0.9)] ${PageSidebar() ? 'max-w-[calc((100vw-320px)*0.9)]' : 'max-w-90'}`}
            ref={containerRef}
            style={{
                'aspect-ratio': DisplayRatio(),
            }}
        >
            <canvas
                class="h-full w-full self-center"
                ref={canvasRef}
                style={{
                    'image-rendering': 'pixelated',
                }}
            />
        </div>
    );
};

export default Canvas;
