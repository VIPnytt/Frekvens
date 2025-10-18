import { mdiBackupRestore, mdiContentSave, mdiDownload, mdiEraser, mdiUpload } from '@mdi/js';
import { Component, createSignal } from 'solid-js';

import { Button } from '../components/Button';
import { Brush, Canvas } from '../components/Canvas';
import { csvExport, fileImport } from '../components/File';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from '../extensions/WebSocket';
import { DisplayColumns, DisplayRows } from '../services/Display';
import { SidebarSection } from '../services/WebServer';

export const name = 'Draw';

const [getFrame, setFrame] = createSignal<number[]>();
const [getSaved, setSaved] = createSignal<boolean>(false);

export const receiver = (json: any) => {
    json[name]?.frame !== undefined && setFrame(json[name].frame);
};

export const Sidebar: Component = () => {
    const { toast } = Toast();

    const handleDownload = () => {
        csvExport(name, [getFrame() || new Array(DisplayColumns() * DisplayRows()).fill(0)]);
    };

    const handleLoad = () => {
        ws.send(JSON.stringify({
            [name]: {
                action: 'load',
            },
        }));
    };

    const handleSave = () => {
        ws.send(JSON.stringify({
            [name]: {
                action: 'save',
            },
        }));
        setSaved(true);
        toast(`Drawing saved`);
    };

    const handleUpload = () => {
        fileImport((frames) => {
            ws.send(JSON.stringify({
                [name]: {
                    frame: frames[0],
                },
            }));
        });
    };

    return (
        <SidebarSection title={name}>
            <Brush />
            <div class="grid grid-cols-2 gap-3">
                <Tooltip text="Save drawing">
                    <Button
                        class="w-full hover:bg-green-600 text-white border-0 px-4 py-3 uppercase text-sm leading-6 tracking-wider cursor-pointer font-bold hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                        disabled={!getFrame()?.some(pixel => pixel > 0) || getSaved()}
                        onClick={handleSave}
                    >
                        <Icon path={mdiContentSave} />
                    </Button>
                </Tooltip>
                <Tooltip text="Restore drawing">
                    <Button
                        class="w-full bg-blue-600 text-white border-0 px-4 py-3 uppercase text-sm leading-6 tracking-wider cursor-pointer font-bold hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                        disabled={!getFrame()?.some(pixel => pixel > 0)}
                        onClick={handleLoad}
                    >
                        <Icon path={mdiBackupRestore} />
                    </Button>
                </Tooltip>
                <Tooltip text="Download drawing">
                    <Button
                        class="w-full bg-blue-600 text-white border-0 px-4 py-3 uppercase text-sm leading-6 tracking-wider cursor-pointer font-bold hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                        disabled={!getFrame()?.some(pixel => pixel > 0)}
                        onClick={handleDownload}
                    >
                        <Icon path={mdiDownload} />
                    </Button>
                </Tooltip>
                <Tooltip text="Upload drawing or image">
                    <Button
                        class="w-full bg-blue-600 text-white border-0 px-4 py-3 uppercase text-sm leading-6 tracking-wider cursor-pointer font-bold hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                        onClick={handleUpload}
                    >
                        <Icon path={mdiUpload} />
                    </Button>
                </Tooltip>
            </div>
        </SidebarSection>
    );
};

export const Main: Component = () => {
    const handleClear = () => {
        setFrame([...new Array(DisplayColumns() * DisplayRows()).fill(0)]);
        ws.send(JSON.stringify({
            [name]: {
                action: 'clear',
            },
        }));
    };

    const handleFrame = (data: number[]) => {
        setSaved(false);
        setFrame([...data]);
    };

    const handlePixel = (_x: number, _y: number, _value: number) => {
        setSaved(false);
        ws.send(JSON.stringify({
            [name]: {
                pixels: [
                    {
                        x: _x,
                        y: _y,
                        value: _value
                    }
                ]
            },
        }));
    };

    const handlePull = () => {
        ws.send(JSON.stringify({
            [name]: {
                action: 'pull',
            },
        }));
    };

    handlePull();

    return (
        <div class="snap-center flex-shrink-0">
            <header class="flex justify-between items-center mb-4">
                <div class="flex gap-3">
                    <Tooltip text="Clear drawing">
                        <Button
                            class="hover:bg-red-600 transition-all"
                            disabled={!getFrame()?.some(pixel => pixel > 0)}
                            onClick={handleClear}
                        >
                            <Icon path={mdiEraser} />
                        </Button>
                    </Tooltip>
                </div>
            </header>
            <div>
                <Canvas
                    onFrame={handleFrame}
                    onPixel={handlePixel}
                    pixels={getFrame() || new Array(DisplayColumns() * DisplayRows()).fill(0)}
                />
            </div>
        </div>
    );
};

export default Main;
