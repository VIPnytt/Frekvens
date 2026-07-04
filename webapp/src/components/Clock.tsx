import {
    mdiClockTimeEightOutline,
    mdiClockTimeElevenOutline,
    mdiClockTimeFiveOutline,
    mdiClockTimeFourOutline,
    mdiClockTimeNineOutline,
    mdiClockTimeOneOutline,
    mdiClockTimeSevenOutline,
    mdiClockTimeSixOutline,
    mdiClockTimeTenOutline,
    mdiClockTimeThreeOutline,
    mdiClockTimeTwelveOutline,
    mdiClockTimeTwoOutline,
} from "@mdi/js";
import { createSignal, onCleanup } from "solid-js";
import { Temporal } from "temporal-polyfill-lite";

const icons = [
    mdiClockTimeTwelveOutline,
    mdiClockTimeOneOutline,
    mdiClockTimeTwoOutline,
    mdiClockTimeThreeOutline,
    mdiClockTimeFourOutline,
    mdiClockTimeFiveOutline,
    mdiClockTimeSixOutline,
    mdiClockTimeSevenOutline,
    mdiClockTimeEightOutline,
    mdiClockTimeNineOutline,
    mdiClockTimeTenOutline,
    mdiClockTimeElevenOutline,
];

const getClockState = () => {
    const now = Temporal.Now.zonedDateTimeISO();
    return {
        icon: icons[(now.minute >= 30 ? now.hour + 1 : now.hour) % icons.length],
        text: new Intl.DateTimeFormat([], {
            hour: "numeric",
            minute: "2-digit",
        }).format(now.epochMilliseconds),
        delay: 60_000 - now.second * 1_000 - now.millisecond,
    };
};

const initialClockState = getClockState();

const [getIcon, setIcon] = createSignal<string>(initialClockState.icon);
const [getText, setText] = createSignal<string>(initialClockState.text);

let timer: ReturnType<typeof setTimeout>;

const update = () => {
    const clockState = getClockState();
    setIcon(clockState.icon);
    setText(clockState.text);
    timer = setTimeout(update, clockState.delay);
};

timer = setTimeout(update, initialClockState.delay);

onCleanup(() => clearTimeout(timer));

export const ClockIcon = getIcon;
export const ClockText = getText;
