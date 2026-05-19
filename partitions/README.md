# 💾 Partition tables

This directory contains predefined partition tables for different ESP32 flash sizes.

> [!NOTE]
> 4MB boards with OTA support are space-constrained. Some feature combinations may not fit. For the full feature set, use the 4MB non-OTA layout with wired flashing, or use an 8MB or larger board.

Use one of the predefined layouts below to match your board’s flash size:

| Flash size | File                   | Feature capacity        | OTA updates        |
| ---------- | ---------------------- | ----------------------- | ------------------ |
|  2 MB      |  `2MB_no_ota_rev2.csv` | :ballot_box_with_check: | :x:                |
|  4 MB      |  `4MB_no_ota.csv`      | :white_check_mark:      | :x:                |
|  4 MB      |  `4MB_rev2.csv`        | :ballot_box_with_check: | :white_check_mark: |
|  8 MB      |  `8MB.csv`             | :white_check_mark:      | :white_check_mark: |
| 16 MB      | `16MB.csv`             | :white_check_mark:      | :white_check_mark: |
| 32 MB      | `32MB.csv`             | :white_check_mark:      | :white_check_mark: |
|  2 MB      |  `2MB_no_ota.csv`      | :warning:               | :x:                |
|  4 MB      |  `4MB.csv`             | :warning:               | :white_check_mark: |

**Legend:**

:white_check_mark: — full

:ballot_box_with_check: — limited

:warning: — legacy / not recommended for new flashes

:x: — not available

> [!TIP]
> Disable unneeded features in the [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) config file if the build is too large to fit. Legacy layouts are kept for compatibility, but are not recommended for new flashes.

## 2️⃣ 2 MB flash boards

The `2MB_no_ota_rev2.csv` layout provides almost the same firmware capacity as `4MB_rev2.csv`, at the cost of [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-ota) support.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/2MB_no_ota_rev2.csv
```

> [!NOTE]
> Revision 2 of the 2 MB table was added in `v2.5.0` to provide a better balance between firmware and filesystem space.

## 4️⃣ 4 MB flash boards

There are two main options, depending on whether OTA updates are required.

### No OTA

The `4MB_no_ota.csv` layout provides the same firmware capacity as `8MB.csv`, at the cost of [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-ota) ability.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB_no_ota.csv
```

### With OTA

The `4MB_rev2.csv` layout provides OTA updates, web app access, and limited firmware capacity.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB_rev2.csv
```

> [!IMPORTANT]
> `4MB_rev2.csv` was added in `v2.5.0` to rebalance NVS, firmware, and filesystem space. Use it for new flashes or wired migrations. Devices already flashed with the original `4MB.csv` layout should keep using `4MB.csv` for OTA updates.

## 8️⃣ 8 MB, 16 MB, and 32 MB flash boards

These provides a comfortable and flexible configuration without any compromises.

Configure in  [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/8MB.csv
```

```ini
board_build.partitions = partitions/16MB.csv
```

```ini
board_build.partitions = partitions/32MB.csv
```
