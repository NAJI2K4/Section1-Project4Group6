# STST-001 & STST-007

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

def test_successful_authentication(monkeypatch):
    mock_sock = MockSocket()
    mock_sock.recv_data = [
        create_packet("AUTH", "testuser|testpass"),
        b""
    ]

    mock_server = MagicMock()
    mock_server.clients = [{"sock": mock_sock, "username": None}]
    mock_server.gui.log = MagicMock()
    mock_server.broadcast = MagicMock()

    monkeypatch.setattr("client_handler.verify_user", lambda u, p: True)

    handler = ClientHandler(mock_sock, ("127.0.0.1", 12345), mock_server)
    handler.run()

    assert any(b"AUTH_OK" in pkt for pkt in mock_sock.sent_data)
    mock_server.gui.log.assert_called_with("[✓] testuser authenticated")

def test_failed_authentication(monkeypatch):
    mock_sock = MockSocket()
    mock_sock.recv_data = [
        create_packet("AUTH", "baduser|wrongpass"),
        b""
    ]

    mock_server = MagicMock()
    mock_server.clients = [{"sock": mock_sock, "username": None}]
    mock_server.gui.log = MagicMock()
    mock_server.broadcast = MagicMock()

    monkeypatch.setattr("client_handler.verify_user", lambda u, p: False)

    handler = ClientHandler(mock_sock, ("127.0.0.1", 12345), mock_server)
    handler.run()

    assert any(b"AUTH_FAIL" in pkt for pkt in mock_sock.sent_data)
    mock_server.gui.log.assert_called_with("[×] Auth failed for baduser")