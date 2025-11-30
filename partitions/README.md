# 💾 Partition tables

This directory contains predefined partition tables for different ESP32 flash memory sizes.

> [!NOTE]
> ESP32 boards come with varying flash sizes. Most older boards have 4 MB, while newer models often include 8 MB. Boards with 2 MB, 16 MB, or 32 MB flash also exist.

Use one of the predefined layouts below to match your board’s flash size:

| Flash size | File              | Feature capacity        | OTA updates        |
| ---------- | ----------------- | ----------------------- | ------------------ |
|  2 MB      |  `2MB_no_ota.csv` | :ballot_box_with_check: | :x:                |
|  4 MB      |  `4MB_no_ota.csv` | :white_check_mark:      | :x:                |
|  4 MB      |  `4MB.csv`        | :ballot_box_with_check: | :white_check_mark: |
|  8 MB      |  `8MB.csv`        | :white_check_mark:      | :white_check_mark: |
| 16 MB      | `16MB.csv`        | :white_check_mark:      | :white_check_mark: |
| 32 MB      | `32MB.csv`        | :white_check_mark:      | :white_check_mark: |

**Legend:**

- :white_check_mark: — full
- :ballot_box_with_check: — limited
- :x: — not available

> [!TIP]
> Disable unneeded features in the [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) config file if the build is too large to fit the partitions.

## 2️⃣ 2 MB flash boards

The `2MB_no_ota.csv` layout provides almost the same firmware capacity as `4MB.csv`, at the cost of [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-ota) support.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/2MB_no_ota_fs.csv
```

## 4️⃣ 4 MB flash boards

Depending on configuration, there’s two alternatives.

### No OTA

The `4MB_no_ota.csv` layout  provides the same firmware capacity as `8MB.csv`, at the cost of the [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-ota) ability.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB_no_ota.csv
```

### With OTA

The `4MB.csv` layout provides a flexible configuration with [OTA](https://github.com/VIPnytt/Frekvens/wiki/Extensions#%EF%B8%8F-ota) updates, [web app](https://github.com/VIPnytt/Frekvens/wiki/Extensions#-web-app) access, and a limited firmware capacity.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB.csv
```

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
