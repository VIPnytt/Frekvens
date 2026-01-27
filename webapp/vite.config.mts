import { cwd } from "node:process";

import { mdiDotsGrid } from "@mdi/js";
import tailwindcss from "@tailwindcss/vite";
import { defineConfig, loadEnv } from "vite";
import { createHtmlPlugin } from "vite-plugin-html";
import { viteSingleFile } from "vite-plugin-singlefile";
import solidPlugin from "vite-plugin-solid";

import { IconUri } from "./src/components/Icon";

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
                            href: IconUri({
                                path: mdiDotsGrid,
                            }),
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
