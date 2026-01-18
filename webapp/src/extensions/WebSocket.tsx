import { createEventSignal } from '@solid-primitives/event-listener';
import { createReconnectingWS, createWSState } from '@solid-primitives/websocket';
import { batch, createEffect, type ParentComponent } from 'solid-js';

import { HOSTNAME } from '../config/constants';
import { EXTENSION_INFRARED, EXTENSION_MESSAGE, EXTENSION_MICROPHONE, EXTENSION_PHOTOCELL, EXTENSION_PLAYLIST, EXTENSION_SIGNAL, MODE_ANIMATION, MODE_COUNTDOWN, MODE_DRAW, MODE_HOMETHERMOMETER, MODE_LARGECLOCK, MODE_SMALLCLOCK, MODE_STREAM, MODE_TICKER } from '../config/modules';
import { name as ExtensionIrName, receiver as ExtensionIrReceiver } from '../extensions/Infrared';
import { name as ExtensionMessageName, receiver as ExtensionMessageReceiver } from '../extensions/Message';
import { name as ExtensionMicName, receiver as ExtensionMicReceiver } from '../extensions/Microphone';
import { name as ExtensionPhotocellName, receiver as ExtensionPhotocellReceiver } from '../extensions/Photocell';
import { name as ExtensionPlaylistName, receiver as ExtensionPlaylistReceiver } from '../extensions/Playlist';
import { name as ExtensionSignalName, receiver as ExtensionSignalReceiver } from '../extensions/Signal';
import { name as ModeAnimationName, receiver as ModeAnimationReceiver } from '../modes/Animation';
import { name as ModeCountdownName, receiver as ModeCountdownReceiver } from '../modes/Countdown';
import { name as ModeDrawName, receiver as ModeDrawReceiver } from '../modes/Draw';
import { name as ModeHomeThermometerName, receiver as ModeHomeThermometerReceiver } from '../modes/HomeThermometer';
import { name as ModeLargeClockName, receiver as ModeLargeClockReceiver } from '../modes/LargeClock';
import { name as ModeSmallClockName, receiver as ModeSmallClockReceiver } from '../modes/SmallClock';
import { name as ModeStreamName, receiver as ModeStreamReceiver } from '../modes/Stream';
import { name as ModeTickerName, receiver as ModeTickerReceiver } from '../modes/Ticker';
import { name as ConnectivityName, receiver as ConnectivityReceiver } from '../services/Connectivity';
import { name as DeviceName, receiver as DeviceReceiver } from '../services/Device';
import { name as DisplayName, receiver as DisplayReceiver } from '../services/Display';
import { name as FontsName, receiver as FontsReceiver } from '../services/Fonts';
import { name as ModesName, receiver as ModesReceiver } from '../services/Modes';
import { Canonical as WebAppCanonical } from './WebApp';

export const name = 'WebSocket';
export const WebSocketUrl = `ws://${HOSTNAME}.local/${name.toLowerCase()}`

export const WebSocketWS = createReconnectingWS(`ws://${import.meta.env.PROD ? location.hostname : `${HOSTNAME}.local`}/${name.toLowerCase()}`);
export const WebSocketState = createWSState(WebSocketWS);

createEffect(() => {
    if (WebSocketState() === WebSocketWS.OPEN) {
        WebAppCanonical();
    }
});

export const WebSocketmessages: ParentComponent = (props) => {
    const event = createEventSignal<{ message: MessageEvent }>(WebSocketWS, 'message');
    createEffect(() => {
        const json = JSON.parse(event()?.data || '{}');
        batch(() => {
            json[ConnectivityName] && ConnectivityReceiver(json[ConnectivityName]);
            json[DeviceName] && DeviceReceiver(json[DeviceName]);
            json[DisplayName] && DisplayReceiver(json[DisplayName]);
            json[FontsName] && FontsReceiver(json[FontsName]);
            json[ModesName] && ModesReceiver(json[ModesName]);
            EXTENSION_INFRARED && json[ExtensionIrName] && ExtensionIrReceiver(json[ExtensionIrName]);
            EXTENSION_MESSAGE && json[ExtensionMessageName] && ExtensionMessageReceiver(json[ExtensionMessageName]);
            EXTENSION_MICROPHONE && json[ExtensionMicName] && ExtensionMicReceiver(json[ExtensionMicName]);
            EXTENSION_PHOTOCELL && json[ExtensionPhotocellName] && ExtensionPhotocellReceiver(json[ExtensionPhotocellName]);
            EXTENSION_PLAYLIST && json[ExtensionPlaylistName] && ExtensionPlaylistReceiver(json[ExtensionPlaylistName]);
            EXTENSION_SIGNAL && json[ExtensionSignalName] && ExtensionSignalReceiver(json[ExtensionSignalName]);
            MODE_ANIMATION && json[ModeAnimationName] && ModeAnimationReceiver(json[ModeAnimationName]);
            MODE_COUNTDOWN && json[ModeCountdownName] && ModeCountdownReceiver(json[ModeCountdownName]);
            MODE_DRAW && json[ModeDrawName] && ModeDrawReceiver(json[ModeDrawName]);
            MODE_HOMETHERMOMETER && json[ModeHomeThermometerName] && ModeHomeThermometerReceiver(json[ModeHomeThermometerName]);
            MODE_LARGECLOCK && json[ModeLargeClockName] && ModeLargeClockReceiver(json[ModeLargeClockName]);
            MODE_TICKER && json[ModeTickerName] && ModeTickerReceiver(json[ModeTickerName]);
            MODE_SMALLCLOCK && json[ModeSmallClockName] && ModeSmallClockReceiver(json[ModeSmallClockName]);
            MODE_STREAM && json[ModeStreamName] && ModeStreamReceiver(json[ModeStreamName]);
        });
    });
    return (props.children);
};
