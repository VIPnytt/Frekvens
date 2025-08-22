import socket
import typing


class ArtNet:
    NAME = "Art-Net"
    sock: typing.Optional[socket.socket] = None

    def __init__(self, host, port=6454, header=18):
        self.host = host
        self.port = port
        self.header = bytearray(header)
        self.sock: typing.Optional[socket.socket] = None

    def __enter__(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if self.sock:
            self.sock.close()
            self.sock = None
        return False

    def send(self, frame):
        if self.sock is None:
            raise RuntimeError("Socket is not open.")
        self.sock.sendto(self.header + bytearray(frame), (self.host, self.port))
