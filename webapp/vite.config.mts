import { mdiDotsGrid } from '@mdi/js';
import tailwindcss from '@tailwindcss/vite'
import { defineConfig, loadEnv } from 'vite';
import { createHtmlPlugin } from 'vite-plugin-html';
import solidPlugin from 'vite-plugin-solid';
import { viteSingleFile } from 'vite-plugin-singlefile';

import { IconUri } from './src/components/Icon';

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
                        href: IconUri({ path: mdiDotsGrid }),
                    },
                },
                {
                    injectTo: 'head' as const,
                    tag: 'title',
                    children: env.NAME,
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
        },
        plugins: [
            solidPlugin(),
            tailwindcss(),
            createHtmlPlugin(createHtml),
            viteSingleFile(),
        ],
    });
});
