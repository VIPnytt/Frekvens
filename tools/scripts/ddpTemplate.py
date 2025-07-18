# Template script for the "Distributed Display Protocol" mode.

import argparse
import socket

import src.config.constants

if src.config.constants.COLUMNS is None:
    src.config.constants.COLUMNS = 16
if src.config.constants.HOST is None:
    src.config.constants.HOST = "frekvens.local"
if src.config.constants.ROWS is None:
    src.config.constants.ROWS = 16

parser = argparse.ArgumentParser(description="Send DDP packet")
parser.add_argument("--host", default=src.config.constants.HOST, help="host, mDNS, hostname or IP")
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
        if not (0 <= x < src.config.constants.COLUMNS and 0 <= y < src.config.constants.ROWS):
            parser.error(f"Pixel overflow: {x}:{y}")
        if not (0 <= brightness < 2**8):
            parser.error(f"Brightness overflow: {brightness}")
        pixels.append((x, y, brightness))

if args.fill is not None:
    pixels = [(x, y, args.fill) for x in range(src.config.constants.COLUMNS) for y in range(src.config.constants.ROWS)]

packet = bytearray([0x41]) + bytearray(10)
data = bytearray([0] * (src.config.constants.COLUMNS * src.config.constants.ROWS * 3))

if pixels:
    for x, y, brightness in pixels:
        if 0 <= x < src.config.constants.COLUMNS and 0 <= y < src.config.constants.ROWS and 0 <= brightness < 2**8:
            index = (x + y * src.config.constants.COLUMNS) * 3
            data[index : index + 3] = [brightness] * 3

packet.extend(data)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(packet, (args.host, 4048))
print(f"Sent DDP packet to {args.host}:4048")
print(f"Packet size: {len(packet)} bytes")
sock.close()
