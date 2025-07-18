import Cookies from 'js-cookie';
import { Component, createSignal } from 'solid-js';

import { Canvas } from '../components/Canvas';
import { ws } from '../extensions/WebSocket';
import { DisplayColumns, DisplayRows } from '../services/Display';

export const name = 'Flies';

const [getFrame, setFrame] = createSignal<number[]>();

export const Main: Component = () => {
    const _id: number = parseInt(Cookies.get(`${name}.id`) ?? '', 10) || Math.round(Math.random() * (Math.pow(2, 8) - 1));
    Cookies.set(`${name}.id`, _id.toString(), {
        expires: 1,
    });

    const handleFrame = (data: number[]) => {
        setFrame([...data]);
    };

    const handlePixel = (_x: number, _y: number, _value: number) => {
        setFrame(new Array(DisplayColumns() * DisplayRows()).fill(0));
        ws.send(JSON.stringify({
            [name]: {
                id: _id,
                x: _x,
                y: _y,
            },
        }));
    };

    return (
        <Canvas
            onFrame={handleFrame}
            onPixel={handlePixel}
            pixels={getFrame() || new Array(DisplayColumns() * DisplayRows()).fill(0)}
        />
    );
};

export default Main;
