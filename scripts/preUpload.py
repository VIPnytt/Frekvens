# Pre-upload script

import os
import subprocess

print("Enter upload method (number 1-3)")
print("1: espota (default)")
print("2: ElegantOTA")
print("3: WiFiManager")
method = input()

Import("env") # type: ignore

match method:
    case 2 | "ElegantOTA":
        env.Replace(UPLOAD_PROTOCOL="custom")  # type: ignore
        subprocess.run(
            [os.path.join(os.path.dirname(__file__), "migration/ElegantOTA.py")]
        )
    case 3 | "WiFiManager":
        env.Replace(UPLOAD_PROTOCOL="custom")  # type: ignore
        subprocess.run(
            [os.path.join(os.path.dirname(__file__), "migration/WiFiManager.py")]
        )
    case _:
        env["UPLOAD_PROTOCOL"]="espota"  # type: ignore
        pass
