import { mdiDotsGrid } from "@mdi/js";
import tailwindcss from "@tailwindcss/vite";
import { defineConfig, loadEnv } from "vite";
import { createHtmlPlugin } from "vite-plugin-html";
import { viteSingleFile } from "vite-plugin-singlefile";
import solidPlugin from "vite-plugin-solid";

import { IconUri } from "./src/components/Icon";

export default defineConfig(({ command, mode, isSsrBuild, isPreview }) => ({
    build: {
        minify: process.env.NODE_ENV === "production",
        target: "esnext",
    },
    define: {
        COMMAND: JSON.stringify(command),
        MODE: JSON.stringify(mode),
        SSR: JSON.stringify(isSsrBuild),
        PREVIEW: JSON.stringify(isPreview),
    },
    plugins: [
        solidPlugin(),
        tailwindcss(),
        createHtmlPlugin({
            inject: {
                tags: [
                    {
                        injectTo: "head" as const,
                        tag: "link",
                        attrs: {
                            rel: "icon",
                            href: IconUri({ path: mdiDotsGrid }),
                        },
                    },
                    {
                        injectTo: "head" as const,
                        tag: "title",
                        children: loadEnv(mode, process.cwd()).VITE_NAME,
                    },
                ],
            },
            minify: process.env.NODE_ENV === "production",
        }),
        viteSingleFile(),
    ],
}));
