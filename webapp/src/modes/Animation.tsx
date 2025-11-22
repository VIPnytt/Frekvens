import { mdiAnimationPlay, mdiBackupRestore, mdiContentSave, mdiDelete, mdiDownload, mdiEraser, mdiPlus, mdiPresentation, mdiPresentationPlay, mdiUpload } from '@mdi/js';
import { Component, createEffect, createSignal, For, Show } from 'solid-js';

import { Strength, Canvas } from '../components/Canvas';
import { csvExport, fileImport } from '../components/File';
import { Icon } from '../components/Icon';
import { Toast } from '../components/Toast';
import { Tooltip } from '../components/Tooltip';
import { Device } from '../config/devices';
import { SidebarSection, WebAppSidebar } from '../extensions/WebApp';
import { WebSocketWS } from '../extensions/WebSocket';
import { MainComponent as ModesMainComponent } from '../services/Modes';

export const name = 'Animation';

type Frame = number[];
type FrameSignal = [() => Frame, (v: Frame) => void];

const [getFrameDuration, setFrameDuration] = createSignal<number>(500);
const [getFrames, setFrames] = createSignal<Frame[]>([]);
const [getFramesDraft, setFramesDraft] = createSignal<FrameSignal[]>([]);
const [getPreview, setPreview] = createSignal<boolean>(false);
const [getPreviewIndex, setPreviewIndex] = createSignal<number>(0);
const [getPreviewTimer, setPreviewTimer] = createSignal<NodeJS.Timeout | undefined>(undefined);
const [getSaved, setSaved] = createSignal<boolean>(true);

export const receiver = (json: any) => {
    json[name]?.duration !== undefined && setFrameDuration(json[name].duration);
    json[name]?.frame !== undefined && json[name]?.index !== undefined && setFrames(f => (f = [...f], f[json[name].index] = json[name].frame, f)) && getSaved() && handleLoad();
};

let ref!: HTMLDivElement;

const empty = new Array(Device.GRID_COLUMNS * Device.GRID_ROWS).fill(0);

const handleLoad = () => {
    if (!getFrames().length) {
        WebSocketWS.send(JSON.stringify({
            [name]: {
                action: 'pull',
            },
        }));
    }
    setFramesDraft(getFrames().map(frame => handleNew(frame)));
    setSaved(true);
};

const handleNew = (initialData?: Frame): FrameSignal => {
    const [frame, setFrame] = createSignal<Frame>(
        initialData ?? empty,
    );
    return [frame, setFrame];
};

const scrollToEnd = () => {
    setTimeout(() => {
        if (ref) {
            ref.scrollTo({
                top: 0,
                left: ref.scrollWidth - ref.clientWidth,
                behavior: 'smooth',
            });
        }
    }, 150);
};

createEffect(() => {
    if (!getPreview()) {
        clearInterval(getPreviewTimer());
        setPreviewTimer(undefined);
        scrollToEnd();
        return;
    }

    let i = 0;
    const cycle = () => {
        setPreviewIndex(i);
        i++;
        if (i >= getFramesDraft().length) {
            i = 0;
        }
    };

    cycle();
    const nodeTimeout = setInterval(cycle, getFrameDuration());
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
        setFramesDraft(list => {
            const lastFrame = list.length ? list[list.length - 1][0]() : undefined;
            const next = handleNew(lastFrame ? [...lastFrame] : undefined);
            scrollToEnd();
            return [...list, next];
        });
    };

    const handleDownload = () => {
        csvExport(name, getFramesDraft().map(([getFrame]) => getFrame()));
    };

    const handleDuration = (time: number) => {
        setFrameDuration(time);
        WebSocketWS.send(JSON.stringify({
            [name]: {
                duration: getFrameDuration(),
            },
        }));
    };

    const handlePreview = () => {
        setPreview(!getPreview());
    };

    const handleSave = async () => {
        const delay = (ms: number): Promise<void> => {
            return new Promise<void>(resolve => setTimeout(resolve, ms));
        }

        const frames = getFramesDraft().map(([frame]) => frame());
        for (let i = 0; i < frames.length; ++i) {
            if (i > 0) {
                await delay(Device.GRID_COLUMNS * Device.GRID_ROWS);
            }
            WebSocketWS.send(JSON.stringify({
                [name]: {
                    frame: frames[i],
                    frames: frames.length,
                    index: i,
                    duration: getFrameDuration(),
                },
            }));
        }
        setSaved(true);
        toast(`${name} saved`);
    };

    const handleUpload = () => {
        fileImport((frames) => {
            frames.forEach(frame => {
                setFramesDraft((signals) => [
                    ...signals,
                    handleNew(frame) as unknown as ReturnType<typeof createSignal<number[]>>,
                ]);
            });
        });
    };

    return (
        <SidebarSection title={name}>
            <div class="grid grid-cols-2 gap-3">
                <Tooltip text={`${getPreview() ? 'Stop' : 'Preview'} animation`}>
                    <button
                        class={`w-full ${getPreview() && 'action-negative not-hover:bg-neutral-light dark:not-hover:bg-neutral-dark not-hover:enabled:text-interactive-light dark:not-hover:enabled:text-content-dark'}`}
                        disabled={getFramesDraft().length < 2 || !getFramesDraft().some(([frame]) => frame().some(pixel => pixel > 0))}
                        onclick={handlePreview}
                    >
                        <Icon path={getPreview() ? mdiPresentationPlay : mdiPresentation} />
                    </button>
                </Tooltip>
                <Tooltip text={`Add frame #${getFramesDraft().length + 1}`}>
                    <button
                        class={`action-neutral w-full ${!getFramesDraft().length && 'not-hover:enabled:bg-neutral-light dark:not-hover:enabled:bg-neutral-dark not-hover:enabled:text-interactive-light dark:not-hover:enabled:text-content-dark'}`}
                        disabled={getPreview()}
                        onclick={handleAdd}
                    >
                        <Icon path={mdiPlus} />
                    </button>
                </Tooltip>
                <Tooltip text="Save animation">
                    <button
                        class="action-neutral w-full"
                        disabled={getFramesDraft().length < 2 || !getFramesDraft().some(([frame]) => frame().some(pixel => pixel > 0)) || getSaved()}
                        onclick={handleSave}
                    >
                        <Icon path={mdiContentSave} />
                    </button>
                </Tooltip>
                <Tooltip text="Restore animation">
                    <button
                        class={`action-neutral w-full ${!getFramesDraft().length && 'not-hover:enabled:bg-neutral-light dark:not-hover:enabled:bg-neutral-dark not-hover:enabled:text-interactive-light dark:not-hover:enabled:text-content-dark'}`}
                        disabled={!getFrames().length || getSaved()}
                        onclick={handleLoad}
                    >
                        <Icon path={mdiBackupRestore} />
                    </button>
                </Tooltip>
                <Tooltip text="Download animation">
                    <button
                        class="w-full"
                        disabled={getFramesDraft().length < 2 || !getFramesDraft().some(([frame]) => frame().some(pixel => pixel > 0))}
                        onclick={handleDownload}
                    >
                        <Icon path={mdiDownload} />
                    </button>
                </Tooltip>
                <Tooltip text="Upload animation, drawing or image">
                    <button
                        class="w-full"
                        disabled={getPreview()}
                        onclick={handleUpload}
                    >
                        <Icon path={mdiUpload} />
                    </button>
                </Tooltip>
            </div>
            <Tooltip text={`${(1_000 / getFrameDuration()).toFixed(1)} fps`}>
                <div class="mt-3 relative">
                    <span class="absolute text-content-alt-light dark:text-content-alt-dark left-3 top-1/2 -translate-y-1/2 text-sm">
                        Frame duration:
                    </span>
                    <input
                        class="text-right pr-14 w-full"
                        disabled={getPreview()}
                        max={Math.pow(2, 16) - 1}
                        name="Duration"
                        min="50"
                        onChange={(e) => handleDuration(parseInt(e.currentTarget.value))}
                        step="10"
                        type="number"
                        value={getFrameDuration()}
                    />
                    <span class="absolute text-content-alt-light dark:text-content-alt-dark right-3 top-1/2 -translate-y-1/2 text-sm">
                        ms
                    </span>
                </div>
            </Tooltip>
            <Strength />
        </SidebarSection>
    );
};

export const Main: Component = () => {
    const handleClear = (index: number) => {
        setFramesDraft((signals) => {
            const [, setFrame] = signals[index];
            setFrame(empty);
            return signals;
        });
        setSaved(false);
    };

    const handleInsert = (index: number) => {
        setFramesDraft((signals) => {
            const [getFrame] = signals[index ? index - 1 : 0];
            const cloned = handleNew([...getFrame()]);
            const updated = [
                ...signals.slice(0, index),
                cloned,
                ...signals.slice(index),
            ];
            setSaved(false);
            return updated;
        });
    };

    const handleRemove = (index: number) => {
        setFramesDraft((signals) => signals.filter((_, i) => i !== index));
        setSaved(false);
    };

    if (!getFramesDraft().length) {
        handleLoad();
    }

    return (
        <Show
            fallback={
                <ModesMainComponent
                    icon={mdiAnimationPlay}
                    text="Add a few frames to get started"
                />
            }
            when={getFramesDraft().length}
        >
            <div class="bg-contrast-light dark:bg-contrast-dark h-full relative rounded-none">
                <div
                    class={`snap-x snap-mandatory flex flex-nowrap overflow-x-auto h-full items-center px-6 gap-[calc((100vw-320px)*0.05)] ${getPreview() ? 'justify-center-safe' : 'justify-start'}`}
                    ref={ref!}
                >
                    {getPreview() ? (
                        <div class="animate-fade-in">
                            <Canvas pixels={getFramesDraft()[getPreviewIndex()]?.[0]() ?? empty} />
                        </div>
                    ) : (
                        <For each={getFramesDraft()}>
                            {([getFrame, setFrame], index) => (
                                <div class={`snap-center flex-shrink-0 max-h-[calc(100vh-128px)] ${WebAppSidebar() ? 'max-w-[calc((100vw-320px)*0.8)]' : 'max-w-[80vw]'}`}>
                                    <header class="flex justify-between items-center mb-4">
                                        <div class="flex gap-3">
                                            <Tooltip text={index() ? `Insert frame between #${index()} and #${index() + 1}` : `Insert frame before #${index() + 1}`}>
                                                <button
                                                    class="canvas-action-neutral"
                                                    onclick={() => handleInsert(index())}
                                                >
                                                    <Icon path={mdiPlus} />
                                                </button>
                                            </Tooltip>
                                            <Tooltip text={`Clear frame #${index() + 1}`}>
                                                <button
                                                    class="canvas-action-negative"
                                                    disabled={!getFramesDraft()[index()][0]?.().some(pixel => pixel > 0)}
                                                    onclick={() => handleClear(index())}
                                                >
                                                    <Icon path={mdiEraser} />
                                                </button>
                                            </Tooltip>
                                            <Tooltip text={`Remove frame #${index() + 1}`}>
                                                <button
                                                    class="canvas-action-negative"
                                                    onclick={() => handleRemove(index())}
                                                >
                                                    <Icon path={mdiDelete} />
                                                </button>
                                            </Tooltip>
                                        </div>
                                        <div class="text-center text-2xl flex items-center">
                                            <span class="font-bold">
                                                {index() + 1}
                                            </span>
                                            <span class="text-content-alt-light dark:text-content-alt-dark">
                                                /{getFramesDraft().length}
                                            </span>
                                        </div>
                                    </header>
                                    <Canvas
                                        onFrame={(data) => {
                                            setSaved(false);
                                            setFrame([...data]);
                                        }}
                                        onPixel={(x, y, value) => {
                                            setSaved(false);
                                            const currentFrame = [...getFrame()];
                                            currentFrame[x + y * Device.GRID_COLUMNS] = value;
                                            setFrame(currentFrame);
                                        }}
                                        pixels={getFrame()}
                                    />
                                </div>
                            )}
                        </For>
                    )}
                </div>
            </div>
        </Show>
    );
};
