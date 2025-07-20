# Migration script for devices handling OTA-updates via the "ayushsharma82/ElegantOTA" or "ayushsharma82/AsyncElegantOTA" libraries.
#
# platformio.ini example config:
"""
upload_protocol = custom
upload_port = 10.0.0.2
extra_scripts = ./scripts/migration/ElegantOTA.py
"""

import hashlib
import os
import requests
import requests_toolbelt
import tqdm


def on_upload(source, target, env):
    file = str(source[0])
    upload_port = env.GetProjectOption("upload_port")

    print(
        "Username for ElegantOTA (enter only if login is required, leave blank otherwise):"
    )
    user = input()
    if len(user):
        print("Password:")
        key = input()
    else:
        user = None
        key = None

    with open(file, "rb") as firmware:
        hash = hashlib.md5(firmware.read()).hexdigest()
        firmware.seek(0)
        encoder = requests_toolbelt.MultipartEncoder(
            fields={
                "firmware": (
                    os.path.split(file)[-1],
                    firmware,
                    "application/octet-stream",
                ),
                "MD5": hash,
            }
        )
        bar = tqdm.tqdm(
            desc="Upload Progress",
            dynamic_ncols=True,
            total=encoder.len,
            unit="B",
            unit_divisor=4096,
            unit_scale=True,
        )
        monitor = requests_toolbelt.MultipartEncoderMonitor(
            encoder, lambda monitor: bar.update(monitor.bytes_read - bar.n)
        )

        if user is not None and key is not None:
            response = requests.post(
                f"http://{upload_port}/update",
                auth=(user, key),
                headers={"Content-Type": monitor.content_type},
                data=monitor,
            )
        else:
            response = requests.post(
                f"http://{upload_port}/update",
                headers={"Content-Type": monitor.content_type},
                data=monitor,
            )

        bar.close()
        print(response, response.text)


Import("env")  # type: ignore
env.Replace(UPLOADCMD=on_upload)  # type: ignore
