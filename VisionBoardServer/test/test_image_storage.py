# STST-002

import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import pytest
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

def test_image_broadcast():
    mock_sock = MockSocket()
    base64_image_data = "testuser|iVBORw0KGgoAAAANSUhEUg..."
    mock_sock.recv_data = [
        create_packet("IMG", base64_image_data),
        b""
    ]

    mock_server = MagicMock()
    mock_server.gui.log = MagicMock()
    mock_server.broadcast = MagicMock()

    handler = ClientHandler(mock_sock, ("127.0.0.1", 12345), mock_server)
    handler.username = "testuser"
    handler.run()

    mock_server.broadcast.assert_called()
    mock_server.gui.log.assert_called_with("[IMG] testuser sent an image.")