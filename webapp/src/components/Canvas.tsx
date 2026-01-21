import { mdiBrightness6 } from "@mdi/js";
import { type Component, createEffect, createSignal, onCleanup, onMount } from "solid-js";

import { Device } from "../config/devices";
import { WebAppSidebar } from "../extensions/WebApp";
import { DisplayBrightness, DisplayOrientation } from "../services/Display";
import { Icon } from "./Icon";
import { Tooltip } from "./Tooltip";

const [getStrength, setStrength] = createSignal<number>(2 ** 8 - 1);

export const Strength: Component = () => (
    <div class="flex items-center mt-3">
        <Icon
            class="mr-2"
            path={mdiBrightness6}
        />
        <Tooltip text={`Brush brightness ${Math.ceil((getStrength() / (2 ** 8 - 1)) * 100)} %`}>
            <input
                class="w-full"
                max={2 ** 8 - 1}
                min="1"
                onInput={(e) => setStrength(parseFloat(e.currentTarget.value))}
                type="range"
                value={getStrength()}
            />
        </Tooltip>
    </div>
);

export const Canvas: Component<{
    disabled?: boolean;
    pixels: number[];
    onFrame?: (data: number[]) => void;
    onPixel?: (x: number, y: number, value: number) => void;
}> = (props) => {
    let canvasRef: HTMLCanvasElement | undefined;
    let divRef: HTMLDivElement | undefined;
    let isDrawing: boolean = false;
    let strength: number = getStrength();

    const rotated = DisplayOrientation() % 180;

    onMount(() => {
        if (canvasRef) {
            canvasRef.addEventListener("pointerdown", handleDown);
            canvasRef.addEventListener("pointerleave", handleUp);
            canvasRef.addEventListener("pointermove", handleMove);
            canvasRef.addEventListener("pointerup", handleUp);
        }
        if (divRef) {
            const resizeObserver = new ResizeObserver(() => {
                draw();
            });
            resizeObserver.observe(divRef);
            onCleanup(() => {
                resizeObserver.disconnect();
            });
        }
        draw();
    });

    onCleanup(() => {
        canvasRef?.removeEventListener("pointerdown", handleDown);
        canvasRef?.removeEventListener("pointerleave", handleUp);
        canvasRef?.removeEventListener("pointermove", handleMove);
        canvasRef?.removeEventListener("pointerup", handleUp);
    });

    createEffect(() => {
        draw();
    });

    const draw = () => {
        if (!canvasRef || !divRef) {
            return;
        }
        canvasRef.width = divRef.clientWidth;
        canvasRef.height = divRef.clientHeight;
        const columnWidth = canvasRef.width / Device.GRID_COLUMNS;
        const rowHeight = canvasRef.height / Device.GRID_ROWS;
        const pixelHeight = rowHeight * (rotated ? Device.LED_SIZE_HORIZONTAL / Device.PITCH_HORIZONTAL : Device.LED_SIZE_VERTICAL / Device.PITCH_VERTICAL);
        const pixelWidth = columnWidth * (rotated ? Device.LED_SIZE_VERTICAL / Device.PITCH_VERTICAL : Device.LED_SIZE_HORIZONTAL / Device.PITCH_HORIZONTAL);
        const offsetX = (columnWidth - pixelWidth) / 2;
        const offsetY = (rowHeight - pixelHeight) / 2;
        const ctx = canvasRef.getContext("2d");
        if (!ctx) {
            return;
        }
        ctx.clearRect(0, 0, canvasRef.width, canvasRef.height);
        ctx.save();
        ctx.globalAlpha = 1;
        ctx.fillStyle = "black";
        ctx.fillRect(0, 0, canvasRef.width, canvasRef.height);
        ctx.globalAlpha = DisplayBrightness() / (2 ** 8 - 1) / 2 + 0.5;
        if (Device.LED_GEOMETRY === "circular") {
            for (let x = 0; x < Device.GRID_COLUMNS; ++x) {
                for (let y = 0; y < Device.GRID_ROWS; ++y) {
                    ctx.fillStyle = toColor(props.pixels[x + y * Device.GRID_COLUMNS]);
                    ctx.beginPath();
                    ctx.arc(offsetX + x * columnWidth + pixelWidth / 2, offsetY + y * rowHeight + pixelHeight / 2, Math.min(pixelHeight, pixelWidth) / 2, 0, 2 * Math.PI);
                    ctx.fill();
                }
            }
        } else if (Device.LED_GEOMETRY === "rectangular") {
            for (let x = 0; x < Device.GRID_COLUMNS; ++x) {
                for (let y = 0; y < Device.GRID_ROWS; ++y) {
                    ctx.fillStyle = toColor(props.pixels[x + y * Device.GRID_COLUMNS]);
                    ctx.fillRect(offsetX + x * columnWidth, offsetY + y * rowHeight, pixelWidth, pixelHeight);
                }
            }
        }
        ctx.restore();
    };

    const toColor = (brightness: number) => {
        const value = brightness === 0 ? Device.LED_BASE_TONE : ((brightness - 1) / (2 ** 8 - 2)) * (2 ** 8 - 2 - Device.LED_BASE_BRIGHTNESS) + 1 + Device.LED_BASE_BRIGHTNESS;
        return `rgb(${value},${value},${value})`;
    };

    const handleEvent = (e: PointerEvent) => {
        if (canvasRef && !props.disabled) {
            const rect = canvasRef.getBoundingClientRect();
            const x = Math.floor(((e.clientX - rect.left) * canvasRef.width) / rect.width / (canvasRef.width / Device.GRID_COLUMNS));
            const y = Math.floor(((e.clientY - rect.top) * canvasRef.height) / rect.height / (canvasRef.height / Device.GRID_ROWS));
            if (x >= 0 && x < Device.GRID_COLUMNS && y >= 0 && y < Device.GRID_ROWS) {
                return {
                    x,
                    y,
                };
            }
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
        const i = position.x + position.y * Device.GRID_COLUMNS;
        strength = props.pixels[i] === 0 ? getStrength() : 0;
        props.onPixel?.(position.x, position.y, strength);
        const newState = props.pixels.map((value, index) => (index === i ? strength : value));
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
        const i = position.x + position.y * Device.GRID_COLUMNS;
        if (props.pixels[i] !== strength) {
            props.onPixel?.(position.x, position.y, strength);

            const newState = props.pixels.map((value, index) => (index === i ? strength : value));
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
            class={`bg-black flex-none inline-block max-h-[calc((100vh---spacing(32))*0.9)] mx-auto p-2.5 relative shrink-0 w-full ${WebAppSidebar() ? "max-w-[calc((100vw---spacing(80))*0.9)]" : "max-w-[90vw]"}`}
            ref={(div) => {
                divRef = div;
            }}
            style={{
                "aspect-ratio": rotated ? `${Device.GRID_ROWS * Device.PITCH_VERTICAL} / ${Device.GRID_COLUMNS * Device.PITCH_HORIZONTAL}` : `${Device.GRID_COLUMNS * Device.PITCH_HORIZONTAL} / ${Device.GRID_ROWS * Device.PITCH_VERTICAL}`,
            }}
        >
            <canvas
                ref={(canvas) => {
                    canvasRef = canvas;
                }}
            />
        </div>
    );
};
