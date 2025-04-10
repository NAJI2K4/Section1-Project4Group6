# STST-004

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

def test_chat_message_relay():
    mock_sock = MockSocket()
    message = "Hello, world!"
    mock_sock.recv_data = [
        create_packet("MSG", message),
        b""
    ]

    mock_server = MagicMock()
    mock_server.gui.log = MagicMock()
    mock_server.broadcast = MagicMock()

    handler = ClientHandler(mock_sock, ("127.0.0.1", 12345), mock_server)
    handler.username = "testuser"
    handler.run()

    assert mock_server.broadcast.call_count == 1
    mock_server.gui.log.assert_called()