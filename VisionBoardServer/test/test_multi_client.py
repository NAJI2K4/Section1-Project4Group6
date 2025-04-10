# STST-008: Multi-Client Messaging Test

import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from unittest.mock import MagicMock
from client_handler import ClientHandler
from packet_utils import create_packet

class MockSocket:
    def __init__(self):
        self.sent_data = []
        self.recv_data = []

    def recv(self, _):
        return self.recv_data.pop(0) if self.recv_data else b""

    def sendall(self, data):
        self.sent_data.append(data)

    def close(self):
        pass

def test_multi_client_chat_broadcast(monkeypatch):
    # Simulate authentication for both users
    monkeypatch.setattr("client_handler.verify_user", lambda u, p: True)

    sock1 = MockSocket()
    sock1.recv_data = [create_packet("AUTH", "user1|pass1"), b""]

    sock2 = MockSocket()
    sock2.recv_data = [create_packet("AUTH", "user2|pass2"), create_packet("MSG", "Hello from user2"), b""]

    server = MagicMock()
    server.clients = [{"sock": sock1, "username": None}, {"sock": sock2, "username": None}]
    server.gui.log = MagicMock()
    server.broadcast = MagicMock()

    handler1 = ClientHandler(sock1, ("127.0.0.1", 10001), server)
    handler2 = ClientHandler(sock2, ("127.0.0.1", 10002), server)

    handler1.run()
    handler2.run()

    # Ensure both clients authenticated and message was broadcast
    assert any(b'AUTH_OK' in pkt for pkt in sock1.sent_data)
    assert any(b'AUTH_OK' in pkt for pkt in sock2.sent_data)
    server.broadcast.assert_any_call(create_packet("MSG", f"[{handler2._time()}] user2: Hello from user2"))
