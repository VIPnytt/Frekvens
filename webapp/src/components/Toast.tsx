import { mdiInformation } from '@mdi/js';
import { createContext, createSignal, ParentComponent, useContext } from 'solid-js';

import { Icon } from './Icon';

type ToastItem = {
    id: number;
    text: string;
    duration: number;
};

const [getToast, setToast] = createSignal<ToastItem | null>(null);

const toast = (text: string, duration: number = 3e3) => {
    const id = Date.now();
    setToast({ id, text, duration });
    setTimeout(() => {
        if (getToast()?.id === id) {
            setToast(null);
        }
    }, duration);
};

const ToastContext = createContext<{
    toast: (text: string, duration?: number) => void;
}>({
    toast,
});

export const ToastProvider: ParentComponent = (props) => {
    const Toast = (props: { item: ToastItem }) => {
        return (
            <div
                class="border bottom-4 bg-base-light dark:bg-base-dark fixed flex items-center left-4 px-4 py-3 rounded-md shadow-lg space-x-2 z-50"
                style="animation: toast 0.15s ease-out forwards;"
                onclick={() => setToast(null)}
            >
                <Icon
                    class="mr-2"
                    path={mdiInformation}
                />
                {props.item.text}
            </div>
        );
    };

    return (
        <ToastContext.Provider value={{ toast }}>
            {getToast() && <Toast item={getToast()!} />}
            {props.children}
        </ToastContext.Provider>
    );
};

export const Toast = () => useContext(ToastContext);
