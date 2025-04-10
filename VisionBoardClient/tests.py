import pytest
import socket
from unittest.mock import patch, MagicMock
from network_handler import NetworkHandler
from chat_window import *
from login_window import *
# Test CTST-008

# pytest -v /Users/nikhilaji/Library/Mobile\ Documents/com~apple~CloudDocs/S4/p4/Vision\ Board\ Project\ 4/VisionBoardClient/tests.py

@pytest.fixture
def network_handler():
    return NetworkHandler()


def test_handle_disconnection(network_handler, qtbot):

    with patch('socket.socket', autospec=True) as mock_socket:
        mock_instance = mock_socket.return_value

        mock_instance.recv.return_value = b''

        with qtbot.waitSignal(network_handler.disconnected, timeout=2000) as blocker:
            network_handler.receive_loop()
        

        assert blocker.signal_triggered


# CTST-002

@pytest.fixture
def chat_window(qtbot, network_handler):
    window = ChatWindow(network_handler, "user1")
    qtbot.addWidget(window)
    return window

def test_send_and_receive_message(chat_window, qtbot):

    chat_window.network.send = MagicMock()


    chat_window.message_input.setText("Hello, world!")
    qtbot.mouseClick(chat_window.send_button, Qt.LeftButton)


    assert chat_window.network.send.called
    packet = chat_window.network.send.call_args[0][0]
    assert "MSG" in packet.decode()  
    assert "Hello, world!" in packet.decode()  

    
    message_data = "MSG|Hello from the server!".encode("utf-8")
    chat_window.receive_data(message_data)

    
    assert chat_window.chat_vbox.count() > 1  
    label = chat_window.chat_vbox.itemAt(0).widget()
    assert label.text() == "Hello from the server!" 

# CTST 009

@pytest.fixture
def login_window(qtbot, network_handler):
    window = LoginWindow(network_handler)
    qtbot.addWidget(window)
    return window

def test_valid_username_password(login_window, qtbot):

    login_window.username.setText("validuser")
    login_window.password.setText("ValidPassword123")
    qtbot.mouseClick(login_window.login_btn, Qt.LeftButton)


    assert login_window.status.text() != "❌ Login failed. Try again."
    assert "Connecting..." in login_window.status.text()  

def test_empty_fields(login_window, qtbot):
    # Simulate empty username and password
    login_window.username.setText("")
    login_window.password.setText("")
    qtbot.mouseClick(login_window.login_btn, Qt.LeftButton)


    assert login_window.status.text() == "Both fields required."

def test_successful_login(login_window, qtbot):

    login_window.network.send = MagicMock()


    login_window.username.setText("validuser")
    login_window.password.setText("ValidPassword123")
    qtbot.mouseClick(login_window.login_btn, Qt.LeftButton)


    login_window.handle_response(b"AUTH_OK")


    assert login_window.isHidden()  
    assert isinstance(login_window.chat_window, ChatWindow)  
