import { type Component, createSignal } from "solid-js";

import { ClockIcon } from "../components/Clock";
import { Toast } from "../components/Toast";
import { Tooltip } from "../components/Tooltip";
import { MODE_SMALLTICKINGCLOCK } from "../config/modules";
import { SidebarSection } from "../extensions/WebApp";
import { WebSocketWS } from "../extensions/WebSocket";
import { name as SmallTickingClockName } from "./LargeTickingClock";
import { MainComponent as ModesMainComponent } from "../services/Modes";

export const name = "Small ticking clock";

const [getTicking, setTicking] = createSignal<boolean>(true);

const { toast } = Toast();

export const Main: Component = () => <ModesMainComponent icon={ClockIcon()} />;

export const Sidebar: Component = () => {
	const handleTicking = (ticking: boolean) => {
		setTicking(ticking);
		WebSocketWS.send(
			JSON.stringify({
				[name]: {
					ticking: getTicking(),
				},
			}),
		);
		toast(`${name} updated`);
	};

	return (
		MODE_SMALLTICKINGCLOCK && (
			<SidebarSection title={name}>
				<div>
					Ticking
					<Tooltip text={SmallTickingClockName}>
						<input
							type="checkbox"
							checked={getTicking()}
							onChange={(e) => handleTicking(e.currentTarget.checked)}
						/>
					</Tooltip>
				</div>
			</SidebarSection>
		)
	);
};
