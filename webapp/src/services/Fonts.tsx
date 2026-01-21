import { createSignal } from "solid-js";

export const name = "Fonts";

const [getFonts, setFonts] = createSignal<string[]>([]);

export const FontsList = getFonts;

export const receiver = (json: { list?: string[] }) => {
    json?.list !== undefined && setFonts(json.list);
};
