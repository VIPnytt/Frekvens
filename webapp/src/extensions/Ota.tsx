import { mdiProgressUpload } from "@mdi/js";
import type { Component } from "solid-js";

import { Icon } from "../components/Icon";
import { Toast } from "../components/Toast";
import { Tooltip } from "../components/Tooltip";
import { BOARD, HOSTNAME, OTA_KEY } from "../config/constants";
import { name as ExtensionsName } from "../services/Extensions";

export const name = "OTA";

export const Link: Component = () => (
    <Tooltip text="Over The Air updates">
        <a
            class="link"
            href={`#/${ExtensionsName.toLowerCase()}/${name.toLowerCase()}`}
        >
            <Icon
                class="mr-2"
                path={mdiProgressUpload}
            />
            {name}
        </a>
    </Tooltip>
);

export const MainThird: Component = () => (
    <div class="main">
        <MainComponent />
    </div>
);

export const MainComponent: Component = () => {
    const { toast } = Toast();

    const handleFile = (
        e: Event & {
            currentTarget: HTMLInputElement;
            target: HTMLInputElement;
        },
    ) => {
        const upload = document.getElementById("upload") as HTMLInputElement;
        if (e.currentTarget.files === null || e.currentTarget.files.length === 0) {
            upload.disabled = true;
            upload.textContent = "Update";
            return;
        }
        upload.disabled = false;
        upload.textContent = e.currentTarget.files[0].name.toLowerCase().includes("littlefs")
            ? "Update Filesystem"
            : "Update Firmware";
    };

    const handleUpload = () => {
        const binary = document.getElementById("binary") as HTMLInputElement;
        if (binary?.files === null || binary.files.length === 0) {
            return;
        }
        binary.disabled = true;
        const form = new FormData();
        form.append("binary", binary.files[0]);
        fetch(`/${name.toLowerCase()}`, {
            method: "POST",
            body: form,
        }).finally(() => {
            toast("Rebooting...", 10e3);
            window.location.reload();
        });
        const upload = document.getElementById("upload") as HTMLInputElement;
        upload.disabled = true;
        upload.classList.add("disabled:bg-negative-alt-light dark:disabled:bg-negative-alt-dark");
        upload.textContent = binary.files[0].name.toLowerCase().includes("littlefs")
            ? "Updating Filesystem..."
            : "Updating Firmware...";
        toast("System update in progress...", 10e3);
    };

    return (
        <div class="space-y-3 p-5">
            <h2>{name}</h2>
            <div class="box">
                <div class="space-y-3">
                    <div class="grid gap-3">
                        {!OTA_KEY && <h3>Automatic updates</h3>}
                        <div class="text-sm">
                            Configure your&nbsp;
                            <a
                                class="italic"
                                href={`https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini`}
                                rel="noopener"
                                target="_blank"
                            >
                                platformio.ini
                            </a>
                            &nbsp;for <span class="italic">over the air</span> updates:
                        </div>
                        <div class="font-mono text-sm  whitespace-nowrap">
                            [env:{BOARD}]
                            <br />
                            board = {BOARD}
                            <br />
                            upload_protocol = espota
                            <br />
                            upload_port = {HOSTNAME}.local
                            {OTA_KEY && (
                                <>
                                    <br />
                                    upload_flags = --auth=password
                                </>
                            )}
                        </div>
                        {!OTA_KEY && (
                            <>
                                <div class="mt-auto border-t" />
                                <h3>Manual update</h3>
                                <div class="text-sm">
                                    Got a pair of <span class="italic">firmware.bin</span> and{" "}
                                    <span class="italic">littlefs.bin</span> files?
                                </div>
                                <input
                                    class="w-full"
                                    id="binary"
                                    type="file"
                                    accept=".bin"
                                    onchange={handleFile}
                                />
                                <button
                                    class="hover:enabled:bg-negative-alt-light dark:hover:enabled:bg-negative-alt-dark mt-3"
                                    id="upload"
                                    disabled
                                    onclick={handleUpload}
                                    type="button"
                                >
                                    Update
                                </button>
                            </>
                        )}
                    </div>
                </div>
            </div>
        </div>
    );
};
