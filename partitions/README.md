# 💾 Partition tables

Partition tables for ESP32 boards with different flash sizes. Choose a layout matching your board, or let the build system select the best fit automatically.

## 📦 Predefined layouts

| Flash size | File                   | Feature capacity        | OTA updates        |
| ---------- | ---------------------- | ----------------------- | ------------------ |
|  2 MB      |  `2MB_no_ota_rev2.csv` | :ballot_box_with_check: | :x:                |
|  4 MB      |  `4MB_no_ota.csv`      | :white_check_mark:      | :x:                |
|  4 MB      |  `4MB_rev2.csv`        | :ballot_box_with_check: | :white_check_mark: |
|  8 MB      |  `8MB.csv`             | :white_check_mark:      | :white_check_mark: |
| 16 MB      | `16MB.csv`             | :white_check_mark:      | :white_check_mark: |
| 32 MB      | `32MB.csv`             | :white_check_mark:      | :white_check_mark: |

**Legend:**

:white_check_mark: — full

:ballot_box_with_check: — limited

:x: — not available

> [!TIP]
> Disable unneeded features in the [.env](https://github.com/VIPnytt/Frekvens/blob/main/.env) config file if the build is too large to fit. OTA support requires space for two firmware images, so disabling OTA is often the most effective option on 4 MB boards.

### Legacy layouts

| Flash size | File              |
| ---------- | ----------------- |
|  2 MB      |  `2MB_no_ota.csv` |
|  4 MB      |  `4MB.csv`        |

These layouts are kept for backward compatibility, but are no longer recommended for new flashes. They were replaced in v2.5.0 by revised layouts with a better balance between firmware and filesystem space.

## 2️⃣ 2 MB flash memory

Boards with only 2 MB flash memory have very limited space, so only a minimal feature set with no OTA-capability is possible.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/2MB_no_ota_rev2.csv
```

## 4️⃣ 4 MB flash memory

There are two main options, depending on whether OTA updates are desired.

### No OTA

The 4 MB flash layout without OTA support is recommended for users who want the full feature set and are comfortable with manual updates.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB_no_ota.csv
```

### With OTA

The 4 MB flash layout with OTA support provides a limited feature set, but allows for convenient over-the-air updates. Recommended for users who prioritize ease of updates and are willing to accept some limitations in features.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB_rev2.csv
```

## 8️⃣ 8 MB+ flash memory

These layouts all include OTA support and provide a comfortable, flexible configuration without compromises.

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

Boards with more than 32 MB flash memory should use the 32 MB layout.
