import { mdiInformation } from '@mdi/js';
import { createContext, createSignal, ParentComponent, useContext } from 'solid-js';

import { Icon } from './Vector';

const [getToast, setToast] = createSignal<{
    text: string;
    duration: number;
} | null>(null);

const toast = (text: string, duration: number = 3e3) => {
    setTimeout(() => {
        setToast({
            text,
            duration,
        });
        setTimeout(() => setToast(null), duration);
    }, 50);
};

const ToastContext = createContext<{
    toast: (text: string, duration?: number) => void;
}>({
    toast,
});

export const ToastProvider: ParentComponent = (props) => {
    const Toast = (props: { text: string }) => {
        return (
            <div
                class="fixed left-4 bottom-4 px-4 py-3 bg-gray-800 text-white shadow-lg border border-gray-700 flex items-center space-x-2 z-50 rounded-md"
                style="animation: toastSlideUp 0.2s ease-out forwards;"
                onClick={() => setToast(null)}
            >
                <Icon
                    class="mr-2"
                    path={mdiInformation}
                />
                {props.text}
            </div>
        );
    };

    return (
        <ToastContext.Provider value={{ toast }}>
            {getToast() !== null && (
                <Toast text={getToast()!.text} />
            )}
            {props.children}
        </ToastContext.Provider>
    );
};

export const Toast = () => useContext(ToastContext);
