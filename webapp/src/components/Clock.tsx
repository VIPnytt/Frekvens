import { mdiClockTimeEightOutline, mdiClockTimeElevenOutline, mdiClockTimeFiveOutline, mdiClockTimeFourOutline, mdiClockTimeNineOutline, mdiClockTimeOneOutline, mdiClockTimeSevenOutline, mdiClockTimeSixOutline, mdiClockTimeTenOutline, mdiClockTimeThreeOutline, mdiClockTimeTwelveOutline, mdiClockTimeTwoOutline } from "@mdi/js";
import { createSignal, onCleanup } from "solid-js";

const clocks = [
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
  return clocks[(now.getMinutes() >= 30 ? hour + 1 : hour) % 12];
};

const [getIcon, setIcon] = createSignal(hour());

const interval = setInterval(() => {
  setIcon(hour());
}, 60_000);

onCleanup(() => clearInterval(interval));

export const ClockIcon = getIcon;