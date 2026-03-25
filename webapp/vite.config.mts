import { cwd } from "node:process";

import { mdiDotsGrid } from "@mdi/js";
import tailwindcss from "@tailwindcss/vite";
import { defineConfig, loadEnv } from "vite";
import { createHtmlPlugin } from "vite-plugin-html";
import { viteSingleFile } from "vite-plugin-singlefile";
import solidPlugin from "vite-plugin-solid";

export default defineConfig(({ mode }) => ({
    build: {
        minify: mode === "production",
        target: "esnext",
    },
    plugins: [
        solidPlugin(),
        tailwindcss(),
        createHtmlPlugin({
            inject: {
                tags: [
                    {
                        injectTo: "head",
                        tag: "link",
                        attrs: {
                            rel: "icon",
                            href: `data:image/svg+xml,${encodeURIComponent(`<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor"><path d="${mdiDotsGrid}"/><style>:root{color:white}@media(prefers-color-scheme:light){:root{color:black}}</style></svg>`)}`,
                        },
                    },
                    {
                        injectTo: "head",
                        tag: "title",
                        children: loadEnv(mode, cwd()).VITE_NAME,
                    },
                ],
            },
            minify: mode === "production",
        }),
        viteSingleFile(),
    ],
}));
