# Migration script for devices handling OTA-updates via the "tzapu/WiFiManager" library.
#
# Get started:
# - Make sure the WiFiManager web user-interface is accessible
# - Upload via PlatformIO
#
# platformio.ini example config:
"""
upload_protocol = custom
upload_port = 192.168.4.1
extra_scripts = ./scripts/migration/WiFiManager.py
"""

import os
import requests


def on_upload(source, target, env):
    file = str(source[0])
    host = env.GetProjectOption("upload_port")

    print("Username for WiFiManager (enter only if login is required, leave blank otherwise):")
    user = input()
    if len(user):
        print("Password:")
        key = input()
    else:
        user = None
        key = None

    with open(file, "rb") as firmware:
        if user is not None and key is not None:
            response = requests.post(
                f"http://{host}/u",
                auth=(user, key),
                files={os.path.split(file)[-1]: firmware},
            )
        else:
            response = requests.post(
                f"http://{host}/u",
                files={os.path.split(file)[-1]: firmware},
            )
        print(response, response.text)


Import("env")  # type: ignore
env.Replace(UPLOADCMD=on_upload)  # type: ignore
