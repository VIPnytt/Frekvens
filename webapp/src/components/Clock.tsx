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

const hour = () => {
    const now = new Date();
    const hour = now.getHours();
    return icons[(now.getMinutes() >= 30 ? hour + 1 : hour) % 12];
};

const text = () =>
    new Date().toLocaleTimeString([], {
        hour: "numeric",
        minute: "2-digit",
    });

const [getIcon, setIcon] = createSignal(hour());
const [getText, setText] = createSignal(text());

let timer: ReturnType<typeof setTimeout>;

const update = () => {
    const now = new Date();
    setIcon(hour());
    setText(text());
    timer = setTimeout(update, 60_000 - now.getSeconds() * 1000 - now.getMilliseconds());
};

update();

onCleanup(() => clearTimeout(timer));

export const ClockIcon = getIcon;
export const ClockText = getText;
