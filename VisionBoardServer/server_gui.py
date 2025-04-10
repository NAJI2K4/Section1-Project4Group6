
from PySide6.QtWidgets import QWidget, QVBoxLayout, QLabel, QPushButton, QListWidget, QTextEdit, QMessageBox
from PySide6.QtCore import QTimer
from server_core import ServerCore

class ServerGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("VisionBoard Server")
        self.setGeometry(100, 100, 500, 600)

        self.server = ServerCore(self)
        self.init_ui()

        self.timer = QTimer()
        self.timer.timeout.connect(self.update_client_list)
        self.timer.start(1000)

    def init_ui(self):
        layout = QVBoxLayout()

        self.status_label = QLabel("Server Running on 127.0.0.1:12345")
        self.client_list = QListWidget()
        self.disconnect_btn = QPushButton("Disconnect Selected Client")
        self.log_view = QTextEdit()
        self.log_view.setReadOnly(True)

        self.disconnect_btn.clicked.connect(self.disconnect_client)

        layout.addWidget(self.status_label)
        layout.addWidget(QLabel("Connected Clients:"))
        layout.addWidget(self.client_list)
        layout.addWidget(self.disconnect_btn)
        layout.addWidget(QLabel("Activity Log:"))
        layout.addWidget(self.log_view)

        self.setLayout(layout)

    def update_client_list(self):
        self.client_list.clear()
        for client in self.server.clients:
            self.client_list.addItem(f"{client['addr']} - {client['username']}")

    def disconnect_client(self):
        selected = self.client_list.currentRow()
        if selected >= 0:
            self.server.disconnect_client(selected)
        else:
            QMessageBox.information(self, "Info", "No client selected.")

    def log(self, message):
        self.log_view.append(message)
