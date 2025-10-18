import { mdiAnimationPlay, mdiBackupRestore, mdiContentSave, mdiDelete, mdiDownload, mdiEraser, mdiPlus, mdiStop, mdiUpload } from '@mdi/js';
import { Component, createEffect, createSignal, For, Show } from 'solid-js';

import { Button } from '../components/Button';
import { Brush, Canvas } from '../components/Canvas';
import { Center } from '../components/Center';
import { csvExport, fileImport } from '../components/File';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Icon } from '../components/Vector';
import { ws } from '../extensions/WebSocket';
import { DisplayColumns, DisplayRows } from '../services/Display';
import { SidebarSection } from '../services/WebServer';
import { PageSidebar } from '../index';

export const name = 'Animation';

const [getFrameDuration, setFrameDuration] = createSignal<number>(500);
const [getFrames, setFrames] = createSignal<[number[]]>();
const [getFrameSignals, setFrameSignals] = createSignal<ReturnType<typeof createSignal<number[]>>[]>([]);
const [getPreview, setPreview] = createSignal<boolean>(false);
const [getPreviewFrame, setPreviewFrame] = createSignal<number[]>();
const [getPreviewTimer, setPreviewTimer] = createSignal<NodeJS.Timeout | string | number | undefined>(undefined);
const [getSaved, setSaved] = createSignal<boolean>(false);

export const receiver = (json: any) => {
    json[name]?.duration !== undefined && setFrameDuration(json[name].duration);
    json[name]?.frames !== undefined && setFrames(json[name].frames);
};

let ref!: HTMLDivElement;

const handleLoad = () => {
    const frames = getFrames();
    if (frames) {
        setFrameSignals(frames.map((frame) =>
            handleNew(frame) as ReturnType<typeof createSignal<number[]>>
        ));
    }
};

const handleNew = (initialData?: number[]) => {
    const [frames, setFrame] = createSignal(initialData || new Array(DisplayColumns() * DisplayRows()).fill(0));
    return [frames, setFrame] as const;
};

const scrollToEnd = () => {
    if (ref) {
        setTimeout(() => {
            ref?.scrollTo({
                top: 0,
                left: ref?.scrollWidth,
                behavior: 'smooth',
            });
        }, 20);
    }
};

createEffect(() => {
    if (!getPreview()) {
        clearInterval(getPreviewTimer());
        setPreviewTimer(undefined);
        scrollToEnd();
        return;
    }

    let i = 0;
    const preview = () => {
        setPreviewFrame(getFrameSignals()[i][0]());
        ++i;
        if (i >= getFrameSignals().length) {
            i = 0;
        }
    };
    preview();
    const nodeTimeout: NodeJS.Timeout | string | number | undefined = setInterval(preview, getFrameDuration());
    setPreviewTimer(nodeTimeout);

    return () => {
        clearInterval(nodeTimeout);
        setPreviewTimer(undefined);
        scrollToEnd();
    };
});

export const Sidebar: Component = () => {
    const { toast } = Toast();

    const handleAdd = () => {
        setFrameSignals((signals): ReturnType<typeof setFrameSignals> => {
            const lastFrame = signals.length > 0 ? signals[signals.length - 1][0]() : undefined;
            const newSignal = handleNew(lastFrame ? [...lastFrame] : undefined);
            scrollToEnd();
            return [
                ...signals,
                newSignal as unknown as ReturnType<typeof createSignal<number[]>>,
            ];
        });
    };

    const handleDownload = () => {
        csvExport(name, getFrameSignals().map(([frames]) => frames()));
    };

    const handleDuration = (time: number) => {
        setFrameDuration(time);
        ws.send(JSON.stringify({
            [name]: {
                duration: getFrameDuration(),
            },
        }));
    };

    const handlePreview = () => {
        setPreview(!getPreview());
    };

    const handleSave = () => {
        const frames = getFrameSignals().map(([frame]) => frame());
        for (let i = 0; i < frames.length; ++i) {
            ws.send(JSON.stringify({
                [name]: {
                    frame: frames[i],
                    index: i,
                    duration: getFrameDuration(),
                    length: frames.length,
                },
            }));
        }
        setSaved(true);
        toast(`${name} saved`);
    };

    const handleUpload = () => {
        fileImport((frames) => {
            frames.forEach(function (frame) {
                setFrameSignals((signals): ReturnType<typeof setFrameSignals> => {
                    return [
                        ...signals,
                        handleNew(frame) as unknown as ReturnType<typeof createSignal<number[]>>,
                    ];
                });
            });
        });
    };

    return (
        <SidebarSection title={name}>
            <Brush />
            <div class="grid grid-cols-2 gap-3">
                <Tooltip text={`${getPreview() ? 'Stop' : 'Preview'} animation`}>
                    <Button
                        class={`transition-all ${getPreview() && ('bg-red-600')}`}
                        disabled={getFrameSignals().length < 2 || !getFrameSignals().some(([frame]) => frame().some(pixel => pixel > 0))}
                        onClick={handlePreview}
                    >
                        <Icon path={getPreview() ? mdiStop : mdiAnimationPlay} />
                    </Button>
                </Tooltip>
                <Tooltip text={`Add frame #${getFrameSignals().length + 1}`}>
                    <Button
                        disabled={getPreview()}
                        onClick={handleAdd}
                    >
                        <Icon path={mdiPlus} />
                    </Button>
                </Tooltip>
                <Tooltip text="Save animation">
                    <Button
                        class="hover:bg-green-600 transition-all"
                        disabled={getFrameSignals().length < 2 || !getFrameSignals().some(([frame]) => frame().some(pixel => pixel > 0)) || getSaved()}
                        onClick={handleSave}
                    >
                        <Icon path={mdiContentSave} />
                    </Button>
                </Tooltip>
                <Tooltip text="Restore animation">
                    <Button
                        class="w-full bg-blue-600 text-white border-0 px-4 py-3 uppercase text-sm leading-6 tracking-wider cursor-pointer font-bold hover:opacity-80 active:translate-y-[-1px] transition-all rounded"
                        disabled={getFrames() === undefined}
                        onClick={handleLoad}
                    >
                        <Icon path={mdiBackupRestore} />
                    </Button>
                </Tooltip>
                <Tooltip text="Download animation">
                    <Button
                        class="hover:bg-blue-600 transition-all"
                        disabled={getFrameSignals().length < 2 || !getFrameSignals().some(([frame]) => frame().some(pixel => pixel > 0))}
                        onClick={handleDownload}
                    >
                        <Icon path={mdiDownload} />
                    </Button>
                </Tooltip>
                <Tooltip text="Upload animation, drawing or image">
                    <Button
                        class="hover:bg-blue-600 transition-all"
                        disabled={getPreview()}
                        onClick={handleUpload}
                    >
                        <Icon path={mdiUpload} />
                    </Button>
                </Tooltip>
            </div>
            <div class="relative">
                <span class="absolute left-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                    Frame duration:
                </span>
                <input
                    class="text-right pl-3 pr-14 py-2 my-1 w-full bg-gray-50 border border-gray-200 rounded-lg focus:ring-2 focus:ring-blue-500 focus:border-blue-500 outline-none transition-all duration-200 disabled:border-0"
                    disabled={getPreview()}
                    max={Math.pow(2, 16) - 1}
                    min="100"
                    onChange={(e) =>
                        handleDuration(parseInt(e.currentTarget.value))
                    }
                    step="10"
                    type="number"
                    value={getFrameDuration()}
                />
                <span class="absolute right-3 top-1/2 -translate-y-1/2 text-gray-400 text-sm">
                    ms
                </span>
            </div>
        </SidebarSection>
    );
};

export const Main: Component = () => {
    const handleClear = (index: number) => {
        setFrameSignals((signals) => {
            if (index !== -1) {
                const [_, setFrame] = signals[index];
                setFrame(new Array(DisplayColumns() * DisplayRows()).fill(0));
            }
            return signals;
        });
    };

    const handleRemove = (index: number) => {
        setFrameSignals((signals) => signals.filter((_, i) => i !== index));
    };

    if (getFrameSignals().length === 0) {
        handleLoad();
    }

    return (
        <div class="h-full relative">
            {getFrameSignals().length ? (
                <div
                    ref={ref!}
                    class={`snap-x snap-mandatory flex flex-nowrap overflow-x-auto h-full items-center px-6 gap-[calc((100vw-320px)*0.05)] ${getPreview() ? 'justify-center' : 'justify-start'}`}
                >
                    {getPreview() ? (
                        <div class="animate-fade-in">
                            <Canvas pixels={getPreviewFrame() || new Array(DisplayColumns() * DisplayRows()).fill(0)} />
                        </div>
                    ) : (
                        <For each={getFrameSignals()}>
                            {([frames, setFrame], index) => (
                                <div class={`snap-center flex-shrink-0 max-h-[calc(100vh-128px)] ${PageSidebar() ? 'max-w-[calc((100vw-320px)*0.5)]' : 'max-w-[50vw]'}`}>
                                    <header class="flex justify-between items-center mb-4">
                                        <div class="flex gap-3">
                                            <Tooltip text={`Clear frame ${index() + 1}`}>
                                                <Button
                                                    class="hover:bg-red-600 transition-all"
                                                    disabled={!getFrameSignals()[index()][0]?.().some(pixel => pixel > 0)}
                                                    onClick={() => handleClear(index())}
                                                >
                                                    <Icon path={mdiEraser} />
                                                </Button>
                                            </Tooltip>
                                            <Tooltip text={`Remove frame ${index() + 1}`}>
                                                <Button
                                                    class="hover:bg-red-600 transition-all w-auto"
                                                    onClick={() => handleRemove(index())}
                                                    widthAuto
                                                >
                                                    <Icon path={mdiDelete} />
                                                </Button>
                                            </Tooltip>
                                        </div>
                                        <div class="text-center text-2xl text-white flex items-center">
                                            <span class="font-bold">{index() + 1}</span>
                                            <span class="opacity-50">/{getFrameSignals().length}</span>
                                        </div>
                                    </header>
                                    <Canvas
                                        onFrame={(data) => {
                                            setSaved(false);
                                            setFrame([...data]);
                                        }}
                                        onPixel={(x, y, value) => {
                                            setSaved(false);
                                            const currentFrame = [...frames()];
                                            currentFrame[x + y * DisplayColumns()] = value;
                                            setFrame(currentFrame);
                                        }}
                                        pixels={frames()}
                                    />
                                </div>
                            )}
                        </For>
                    )}
                </div>
            ) : (
                <Center>
                    <h2 class="text-4xl">
                        <Icon path={mdiAnimationPlay} />
                    </h2>
                    <p class="text-xl mt-2 text-gray-300">
                        {name}
                    </p>
                    <p class="text-sm mt-2 text-gray-300">
                        Add the first frame to get started
                    </p>
                </Center>
            )}
        </div>
    );
};

export default Main;
