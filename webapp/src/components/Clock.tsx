import { mdiClockTimeEightOutline, mdiClockTimeElevenOutline, mdiClockTimeFiveOutline, mdiClockTimeFourOutline, mdiClockTimeNineOutline, mdiClockTimeOneOutline, mdiClockTimeSevenOutline, mdiClockTimeSixOutline, mdiClockTimeTenOutline, mdiClockTimeThreeOutline, mdiClockTimeTwelveOutline, mdiClockTimeTwoOutline } from "@mdi/js";
import { Component, createSignal, onCleanup } from "solid-js";

import { MainComponent as ModesMainComponent } from '../services/Modes';

const clocks = [mdiClockTimeTwelveOutline, mdiClockTimeOneOutline, mdiClockTimeTwoOutline, mdiClockTimeThreeOutline, mdiClockTimeFourOutline, mdiClockTimeFiveOutline, mdiClockTimeSixOutline, mdiClockTimeSevenOutline, mdiClockTimeEightOutline, mdiClockTimeNineOutline, mdiClockTimeTenOutline, mdiClockTimeElevenOutline];

export const MainComponent: Component = () => {
    const hour = () => {
        const now = new Date();
        const h = now.getHours();
        return clocks[(now.getMinutes() >= 30 ? h + 1 : h) % 12];
    };

    const [getIcon, setIcon] = createSignal(hour());

    const interval = setInterval(() => {
        setIcon(hour());
    }, 60_000);

    onCleanup(() => clearInterval(interval));

    return (
        <ModesMainComponent
            icon={getIcon()}
        />
    )
}
