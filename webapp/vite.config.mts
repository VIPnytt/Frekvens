import { mdiDotsGrid } from '@mdi/js';
import tailwindcss from '@tailwindcss/vite'
import { defineConfig, loadEnv } from 'vite';
import { createHtmlPlugin } from 'vite-plugin-html';
import solidPlugin from 'vite-plugin-solid';
import { viteSingleFile } from 'vite-plugin-singlefile';

import { IconUri } from './src/components/Vector';
import libraryJson from '../library.json';
import packageJson from './package-lock.json';

export default defineConfig(({ command, mode, isSsrBuild, isPreview }) => {
    const env = loadEnv(mode, process.cwd(), '');
    const createHtml = {
        inject: {
            tags: [
                {
                    injectTo: 'head' as const,
                    tag: 'link',
                    attrs: {
                        rel: 'icon',
                        type: 'image/svg+xml',
                        href: IconUri(mdiDotsGrid),
                    },
                },
                {
                    injectTo: 'head' as const,
                    tag: 'link',
                    attrs: {
                        rel: 'license',
                        href: `${libraryJson.homepage}/blob/main/LICENSE`,
                    },
                },
                {
                    injectTo: 'head' as const,
                    tag: 'title',
                    children: env.NAME || "",
                },
            ],
        },
        minify: process.env.NODE_ENV === 'production',
    };

    return ({
        build: {
            minify: process.env.NODE_ENV === 'production',
            target: 'esnext',
        },
        define: {
            'COMMAND': JSON.stringify(command),
            'MODE': JSON.stringify(mode),
            'SSR': JSON.stringify(isSsrBuild),
            'PREVIEW': JSON.stringify(isPreview),
            'REPOSITORY': JSON.stringify(libraryJson.homepage),
            '__VERSION__': JSON.stringify(packageJson.version),
        },
        plugins: [
            solidPlugin(),
            tailwindcss(),
            createHtmlPlugin(createHtml),
            viteSingleFile(),
        ],
    });
});
