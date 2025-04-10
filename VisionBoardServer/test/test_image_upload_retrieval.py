# STST-003: Image Upload & Retrieval Test

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

def test_image_upload_retrieval():
    # Simulate receiving an image packet from the client
    mock_sock = MockSocket()
    encoded_image = "username|base64encodedimage..."
    mock_sock.recv_data = [
        create_packet("IMG", encoded_image),
        b""  # Simulate end of connection
    ]

    # Mock server and dependencies
    mock_server = MagicMock()
    mock_server.gui.log = MagicMock()
    mock_server.broadcast = MagicMock()

    # Initialize handler and simulate image upload
    handler = ClientHandler(mock_sock, ("127.0.0.1", 12345), mock_server)
    handler.username = "username"
    handler.run()

    # Assert that broadcast was called with correct image packet (no exclude)
    mock_server.broadcast.assert_called_with(create_packet("IMG", encoded_image))