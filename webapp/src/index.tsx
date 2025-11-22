import { render } from 'solid-js/web';

import { WebApp } from './extensions/WebApp';

import './style.css';

render(
    () => (
        <WebApp />
    ),
    document.getElementById('webapp') as HTMLElement,
);
