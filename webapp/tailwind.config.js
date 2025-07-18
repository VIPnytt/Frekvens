const plugin = require('tailwindcss/plugin');

/** @type {import('tailwindcss').Config} */
module.exports = {
    content: [
        './src/**/*.{js,jsx,ts,tsx}',
    ],
    darkMode: [
        'class',
    ],
    plugins: [
        require('tailwindcss-animate'),
    ],
    theme: {},
};
