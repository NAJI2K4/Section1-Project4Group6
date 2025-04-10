from PySide6.QtWidgets import QWidget, QVBoxLayout, QLineEdit, QPushButton, QLabel
from PySide6.QtCore import Signal
from utils import create_raw_packet

class LoginWindow(QWidget):
    def __init__(self, network_handler):
        super().__init__()
        self.network = network_handler
        self.setWindowTitle("VisionBoard - Login")

        self.layout = QVBoxLayout()
        self.status = QLabel("Connecting...")
        self.username = QLineEdit()
        self.username.setPlaceholderText("Username")

        self.password = QLineEdit()
        self.password.setPlaceholderText("Password")
        self.password.setEchoMode(QLineEdit.Password)

        self.login_btn = QPushButton("Login")

        self.layout.addWidget(self.status)
        self.layout.addWidget(self.username)
        self.layout.addWidget(self.password)
        self.layout.addWidget(self.login_btn)
        self.setLayout(self.layout)

        self.login_btn.clicked.connect(self.send_login)
        self.network.connected.connect(lambda: self.status.setText("Connected"))
        self.network.data_received.connect(self.handle_response)

    def send_login(self):
        username = self.username.text()
        password = self.password.text()
        if not username or not password:
            self.status.setText("Both fields required.")
            return

        # Build raw AUTH packet
        packet = create_raw_packet("AUTH", f"{username}|{password}")
        self.network.send(packet)

    def handle_response(self, data):
        if b"AUTH_OK" in data:
            from chat_window import ChatWindow
            self.chat_window = ChatWindow(self.network, self.username.text())
            self.chat_window.show()
            self.close()
        elif b"AUTH_FAIL" in data:
            self.status.setText("❌ Login failed. Try again.")
