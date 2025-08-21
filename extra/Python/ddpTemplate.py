# Template script for the "Distributed Display Protocol" mode.

import argparse
import socket

from tools.src.config.constants import COLUMNS, HOST, ROWS

if not COLUMNS:
    COLUMNS = 16
if not HOST:
    HOST = "frekvens.local"
if not ROWS:
    ROWS = 16

parser = argparse.ArgumentParser(description="Send DDP packet")
parser.add_argument("--host", default=HOST, help="host, mDNS, hostname or IP")
parser.add_argument("--clear", action="store_true", help="Clear display")
parser.add_argument(
    "--fill",
    type=int,
    metavar="BRIGHTNESS",
    help="Fill display",
)
parser.add_argument(
    "--pixel",
    nargs=3,
    type=int,
    action="append",
    metavar=("X", "Y", "BRIGHTNESS"),
    help="Set pixel X:Y to brightness",
)

args = parser.parse_args()

if args.fill is not None and not 0 <= args.fill < 2**8:
    parser.error(f"Brightness overflow: {args.fill}")

pixels = []
if args.pixel:
    for x, y, brightness in args.pixel:
        if not (0 <= x < COLUMNS and 0 <= y < ROWS):
            parser.error(f"Pixel overflow: {x}:{y}")
        if not (0 <= brightness < 2**8):
            parser.error(f"Brightness overflow: {brightness}")
        pixels.append((x, y, brightness))

if args.fill is not None:
    pixels = [(x, y, args.fill) for x in range(COLUMNS) for y in range(ROWS)]

packet = bytearray([0x41]) + bytearray(10)
data = bytearray([0] * (COLUMNS * ROWS * 3))

if pixels:
    for x, y, brightness in pixels:
        if 0 <= x < COLUMNS and 0 <= y < ROWS and 0 <= brightness < 2**8:
            index = (x + y * COLUMNS) * 3
            data[index : index + 3] = [brightness] * 3

packet.extend(data)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (args.host, 4048))
print(f"Sent DDP packet to {args.host}:4048")
print(f"Packet size: {len(packet)} bytes")
sock.close()
