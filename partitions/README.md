# 💾 Partition tables

Partition tables for ESP32 boards with different flash sizes. Choose a layout matching your board, or let the build system select the best fit automatically.

## 📦 Predefined layouts

| Flash size | File                   | OTA updates        |
| ---------- | ---------------------- | ------------------ |
|  4 MB      |  `4MB_no_ota.csv`      | :x:                |
|  8 MB      |  `8MB.csv`             | :white_check_mark: |
| 16 MB      | `16MB.csv`             | :white_check_mark: |
| 32 MB      | `32MB.csv`             | :white_check_mark: |

## 4️⃣ 4 MB flash memory

Boards with 4 MB flash memory have no OTA support due to space limitations.

Configure in [platformio.ini](https://github.com/VIPnytt/Frekvens/blob/main/platformio.ini):

```ini
board_build.partitions = partitions/4MB_no_ota.csv
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
