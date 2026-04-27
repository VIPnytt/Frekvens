import { mdiCellphoneScreenshot, mdiDownload } from "@mdi/js";
import { type Component, createSignal } from "solid-js";

import { Canvas } from "../components/Canvas";
import { csvExport } from "../components/File";
import { Icon } from "../components/Icon";
import { Tooltip } from "../components/Tooltip";
import { Device } from "../config/devices";
import { DisplayPower } from "../services/Display";
import { name as ExtensionsName } from "../services/Extensions";
import { ModesMode } from "../services/Modes";
import { WebSocketWS } from "./WebSocket";

export const name = "Screenshot";

const [getFrame, setFrame] = createSignal<number[]>();

export const receiver = (json: { frame?: number[] }) => {
    json?.frame !== undefined && setFrame(json.frame);
};

export const Link: Component = () => (
    <a
        class="link"
        href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
    >
        <Icon
            class="mr-2"
            path={mdiCellphoneScreenshot}
        />
        {name}
    </a>
);

export const MainThird: Component = () => {
    const handleDownload = () => {
        csvExport(ModesMode().toLowerCase() || name.toLowerCase(), [
            getFrame() || new Array(Device.GRID_COLUMNS * Device.GRID_ROWS).fill(0),
        ]);
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

    if (!getFrame()?.some((pixel) => pixel > 0)) {
        handlePull();
    }

    return (
        <div class="bg-contrast-light dark:bg-contrast-dark main">
            <div class="shrink-0 snap-center">
                <header class="flex justify-between items-center mb-4">
                    <div class="flex gap-3">
                        <Tooltip text="Take screenshot">
                            <button
                                class="canvas-action-neutral"
                                disabled={!DisplayPower()}
                                onclick={handlePull}
                                type="button"
                            >
                                <Icon path={mdiCellphoneScreenshot} />
                            </button>
                        </Tooltip>
                        <Tooltip text="Download">
                            <button
                                class="canvas-action-neutral"
                                disabled={!getFrame()?.some((pixel) => pixel > 0)}
                                onclick={handleDownload}
                                type="button"
                            >
                                <Icon path={mdiDownload} />
                            </button>
                        </Tooltip>
                    </div>
                </header>
                <div>
                    <Canvas
                        disabled={true}
                        pixels={getFrame() || new Array(Device.GRID_COLUMNS * Device.GRID_ROWS).fill(0)}
                    />
                </div>
            </div>
        </div>
    );
};
