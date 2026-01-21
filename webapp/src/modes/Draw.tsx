import { mdiBackupRestore, mdiContentSave, mdiDownload, mdiEraser, mdiUpload } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { Canvas, Strength } from "../components/Canvas";
import { csvExport, fileImport } from "../components/File";
import { Icon } from "../components/Icon";
import { Toast } from "../components/Toast";
import { Tooltip } from "../components/Tooltip";
import { Device } from "../config/devices";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";

export const name = "Draw";

const [getFrame, setFrame] = createSignal<number[]>();
const [getSaved, setSaved] = createSignal<boolean>(false);

export const receiver = (json: { frame?: number[] }) => {
    json?.frame !== undefined && setFrame(json.frame);
};

export const Sidebar: Component = () => {
    const { toast } = Toast();

    const handleDownload = () => {
        csvExport(name, [
            getFrame() || new Array(Device.GRID_COLUMNS * Device.GRID_ROWS).fill(0),
        ]);
    };

    const handleLoad = () => {
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    action: "load",
                },
            }),
        );
        setSaved(true);
    };

    const handleSave = () => {
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    action: "save",
                },
            }),
        );
        setSaved(true);
        toast(`Drawing saved`);
    };

    const handleUpload = () => {
        fileImport((frames) => {
            setFrame(frames[0]);
            WebSocketWS.send(
                JSON.stringify({
                    [name]: {
                        frame: frames[0],
                    },
                }),
            );
            setSaved(false);
        });
    };

    return (
        <SidebarSection title={name}>
            <div class="grid grid-cols-2 gap-3">
                <Tooltip text="Save drawing">
                    <button
                        class="action-positive w-full"
                        disabled={!getFrame()?.some((pixel) => pixel > 0) || getSaved()}
                        onclick={handleSave}
                        type="button"
                    >
                        <Icon path={mdiContentSave} />
                    </button>
                </Tooltip>
                <Tooltip text="Restore drawing">
                    <button
                        class="action-neutral w-full"
                        disabled={getSaved()}
                        onclick={handleLoad}
                        type="button"
                    >
                        <Icon path={mdiBackupRestore} />
                    </button>
                </Tooltip>
                <Tooltip text="Download drawing">
                    <button
                        class="w-full"
                        disabled={!getFrame()?.some((pixel) => pixel > 0)}
                        onclick={handleDownload}
                        type="button"
                    >
                        <Icon path={mdiDownload} />
                    </button>
                </Tooltip>
                <Tooltip text="Upload drawing or image">
                    <button
                        class="w-full"
                        onclick={handleUpload}
                        type="button"
                    >
                        <Icon path={mdiUpload} />
                    </button>
                </Tooltip>
            </div>
            <Strength />
        </SidebarSection>
    );
};

export const Main: Component = () => {
    const handleClear = () => {
        setFrame([
            ...new Array(Device.GRID_COLUMNS * Device.GRID_ROWS).fill(0),
        ]);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    action: "clear",
                },
            }),
        );
    };

    const handleFrame = (data: number[]) => {
        setSaved(false);
        setFrame([
            ...data,
        ]);
    };

    const handlePixel = (x: number, y: number, brightness: number) => {
        setSaved(false);
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    pixels: [
                        {
                            x: x,
                            y: y,
                            brightness: brightness,
                        },
                    ],
                },
            }),
        );
    };

    const handlePull = () => {
        WebSocketWS.send(
            JSON.stringify({
                [name]: {
                    action: "pull",
                },
            }),
        );
    };

    handlePull();

    return (
        <div class="bg-contrast-light dark:bg-contrast-dark main">
            <div class="shrink-0 snap-center">
                <header class="flex justify-between items-center mb-4">
                    <div class="flex gap-3">
                        <Tooltip text="Clear drawing">
                            <button
                                class="canvas-action-negative"
                                disabled={!getFrame()?.some((pixel) => pixel > 0)}
                                onclick={handleClear}
                                type="button"
                            >
                                <Icon path={mdiEraser} />
                            </button>
                        </Tooltip>
                    </div>
                </header>
                <div>
                    <Canvas
                        onFrame={handleFrame}
                        onPixel={handlePixel}
                        pixels={getFrame() || new Array(Device.GRID_COLUMNS * Device.GRID_ROWS).fill(0)}
                    />
                </div>
            </div>
        </div>
    );
};
