import { mdiDotsGrid } from "@mdi/js";
import tailwindcss from "@tailwindcss/vite";
import { defineConfig, loadEnv } from "vite";
import { viteSingleFile } from "vite-plugin-singlefile";
import solidPlugin from "vite-plugin-solid";

export default defineConfig(({ mode }) => ({
    plugins: [
        {
            name: "html-transform",
            enforce: "pre",
            transformIndexHtml: {
                handler() {
                    return [
                        {
                            tag: "link",
                            injectTo: "head",
                            attrs: {
                                rel: "icon",
                                href: `data:image/svg+xml,${encodeURIComponent(`<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" fill="currentColor"><path d="${mdiDotsGrid}"/><style>:root{color:white}@media(prefers-color-scheme:light){:root{color:black}}</style></svg>`)}`,
                            },
                        },
                        {
                            tag: "title",
                            injectTo: "head",
                            children: loadEnv(mode, ".").VITE_NAME,
                        },
                    ];
                },
            },
        },
        solidPlugin(),
        tailwindcss(),
        viteSingleFile(),
        {
            name: "html-minify",
            enforce: "post",
            generateBundle(_, bundle) {
                for (const file of Object.values(bundle)) {
                    if (file.type === "asset" && file.fileName === "index.html") {
                        file.source = file.source.toString().replace(/^\s+/gm, "").trim();
                    }
                }
            },
        },
    ],
}));
