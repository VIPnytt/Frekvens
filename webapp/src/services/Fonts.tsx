import { createSignal } from 'solid-js';

export const name = 'Fonts';

const [getFonts, setFonts] = createSignal<string[]>([]);

export const FontsList = getFonts;

export const receiver = (json: any) => {
    json[name]?.list !== undefined && setFonts(json[name].list);
};

export default FontsList;
