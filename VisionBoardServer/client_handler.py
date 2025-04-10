"""
# client_handler.py
from user_db import verify_user
from packet_utils import parse_packet, create_packet
from datetime import datetime

class ClientHandler:
    def __init__(self, sock, addr, server):
        self.sock = sock
        self.addr = addr
        self.server = server
        self.username = "Unknown"

    def run(self):
        try:
            while True:
                data = self.sock.recv(4096)
                if not data:
                    break
                packet_type, payload = parse_packet(data)

                if packet_type == "AUTH":
                    uname, pwd = payload.split("|")
                    if verify_user(uname, pwd):
                        self.username = uname
                        for c in self.server.clients:
                            if c["sock"] == self.sock:
                                c["username"] = uname
                        self.sock.sendall(create_packet("AUTH_OK", ""))
                        self.server.broadcast(create_packet("MSG", f"System: {uname} joined the chat."))
                        self.server.gui.log(f"[\u2713] {uname} authenticated")
                    else:
                        self.sock.sendall(create_packet("AUTH_FAIL", ""))
                        self.server.gui.log(f"[\u00d7] Auth failed for {uname}")

                elif packet_type == "MSG":
                    timestamped = f"[{self.get_time()}] {self.username}: {payload}"
                    self.server.broadcast(create_packet("MSG", timestamped))
                    self.server.gui.log(timestamped)

                elif packet_type == "IMG":
                    # payload is already base64 string
                     self.server.broadcast(create_packet("IMG", f"{self.username}|{payload}"), exclude=self.sock)
                     self.server.gui.log(f"[IMG] {self.username} sent an image.")


        except Exception as e:
            self.server.gui.log(f"[!] Error with {self.addr}: {e}")
        finally:
            self.sock.close()

    def get_time(self):
        return datetime.now().strftime("%H:%M:%S")
"""
from user_db import verify_user
from packet_utils import parse_packet, create_packet
from datetime import datetime

class ClientHandler:
    def __init__(self, sock, addr, server):
        self.sock = sock
        self.addr = addr
        self.server = server
        self.username = "Unknown"

    def run(self):
        try:
            while True:
                data = self.sock.recv(40960)
                if not data:
                    break
                packet_type, payload = parse_packet(data)

                if packet_type == "AUTH":
                    uname, pwd = payload.split("|")
                    if verify_user(uname, pwd):
                        self.username = uname
                        for c in self.server.clients:
                            if c["sock"] == self.sock:
                                c["username"] = uname
                        self.sock.sendall(create_packet("AUTH_OK", ""))
                        self.server.broadcast(create_packet("MSG", f"[{self._time()}] System: {uname} joined the chat."))
                        self.server.gui.log(f"[\u2713] {uname} authenticated")
                    else:
                        self.sock.sendall(create_packet("AUTH_FAIL", ""))
                        self.server.gui.log(f"[\u00d7] Auth failed for {uname}")

                elif packet_type == "MSG":
                    timestamped = f"[{self._time()}] {self.username}: {payload}"
                    self.server.broadcast(create_packet("MSG", timestamped))
                    self.server.gui.log(timestamped)

                elif packet_type == "IMG":
                    self.server.broadcast(create_packet("IMG", payload))  # full payload is already labeled + base64
                    self.server.gui.log(f"[IMG] {self.username} sent an image.")

        except Exception as e:
            self.server.gui.log(f"[!] Error with {self.addr}: {e}")
        finally:
            self.sock.close()

    def _time(self):
        return datetime.now().strftime("%H:%M:%S")
