import socket
import threading
from PySide6.QtCore import Signal, QObject

class NetworkHandler(QObject):
    connected = Signal()
    disconnected = Signal()
    data_received = Signal(bytes)

    def __init__(self):
        super().__init__()
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.host = '127.0.0.1'
        self.port = 12345
        self.receive_thread = None

    def connect_to_server(self):
        try:
            self.client_socket.connect((self.host, self.port))
            self.connected.emit()
            self.receive_thread = threading.Thread(target=self.receive_loop, daemon=True)
            self.receive_thread.start()
        except Exception as e:
            print(f"Connection failed: {e}")

    def send(self, data: bytes):
        try:
            self.client_socket.sendall(data)
        except Exception as e:
            print(f"Send failed: {e}")

    def receive_loop(self):
        try:
            while True:
                data = self.client_socket.recv(4096)
                if data:
                    self.data_received.emit(data)
                else:
                    self.disconnected.emit()
                    break
        except Exception as e:
            print(f"Receive failed: {e}")
            self.disconnected.emit()