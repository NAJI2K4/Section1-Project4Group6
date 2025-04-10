from PySide6.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QLabel, QLineEdit, QPushButton,
    QFileDialog, QScrollArea, QFrame, QSizePolicy
)
from PySide6.QtGui import QPixmap, QColor, QFont
from PySide6.QtCore import Qt, QTimer
from annotator import Annotator
from utils import create_packet
from datetime import datetime
import base64

class ChatWindow(QWidget):
    def __init__(self, network_handler, username):
        super().__init__()
        self.network = network_handler
        self.username = username
        self.setWindowTitle("VisionBoard - Chat")

        layout = QHBoxLayout()
        chat_layout = QVBoxLayout()

        # Scrollable chat area
        self.scroll_area = QScrollArea()
        self.scroll_area.setWidgetResizable(True)
        self.chat_container = QWidget()
        self.chat_vbox = QVBoxLayout(self.chat_container)
        self.chat_vbox.addStretch(1)
        self.scroll_area.setWidget(self.chat_container)

        self.message_input = QLineEdit()
        self.send_button = QPushButton("Send")
        self.clear_button = QPushButton("Clear Chat")

        chat_layout.addWidget(QLabel("Chat"))
        chat_layout.addWidget(self.scroll_area)
        chat_layout.addWidget(self.message_input)
        chat_layout.addWidget(self.send_button)
        chat_layout.addWidget(self.clear_button)

        # Right side: Annotator tools
        right_layout = QVBoxLayout()
        self.annotator = Annotator()
        self.upload_button = QPushButton("Upload Image")
        self.send_image_button = QPushButton("Send Image")

        right_layout.addWidget(QLabel("Image Tools"))
        right_layout.addWidget(self.annotator)
        right_layout.addWidget(self.upload_button)
        right_layout.addWidget(self.send_image_button)

        layout.addLayout(chat_layout, 2)
        layout.addLayout(right_layout, 1)
        self.setLayout(layout)

        self.send_button.clicked.connect(self.send_message)
        self.upload_button.clicked.connect(self.load_image)
        self.send_image_button.clicked.connect(self.send_image)
        self.clear_button.clicked.connect(self.clear_chat)
        self.network.data_received.connect(self.receive_data)

    def send_message(self):
        msg = self.message_input.text()
        if msg:
            packet = create_packet("MSG", self.username, msg)
            self.network.send(packet)
            self.message_input.clear()

    def load_image(self):
        path, _ = QFileDialog.getOpenFileName(self, "Select Image")
        if path:
            self.annotator.load_image(path)

    def send_image(self):
        image_bytes = self.annotator.get_image_bytes()
        if image_bytes:
            encoded = base64.b64encode(image_bytes).decode('utf-8')
            timestamp = datetime.now().strftime("%H:%M")
            label = f"{self.username} ({timestamp})"
            full_payload = f"{label}|{encoded}"
            packet = f"IMG|{full_payload}"
            self.network.send(packet.encode())

    def receive_data(self, data):
        try:
            decoded = data.decode(errors='ignore')
            if decoded.startswith("MSG|"):
                _, payload = decoded.split("|", 1)
                self.add_chat_bubble(payload)

            elif decoded.startswith("IMG|"):
                try:
                    _, payload = decoded.split("IMG|", 1)
                    label, encoded = payload.split("|", 1)
                    image_bytes = base64.b64decode(encoded.encode('utf-8'))
                    pixmap = QPixmap()
                    if pixmap.loadFromData(image_bytes):
                        self.annotator.setPixmap(pixmap)
                        self.add_image_bubble(label, pixmap)
                    else:
                        print("[CLIENT] QPixmap load failed.")
                except Exception as e:
                    print("[CLIENT] Failed to decode image:", e)
        except Exception as e:
            print("[CLIENT] Failed to decode packet:", e)

    def add_chat_bubble(self, text):
        label = QLabel(text)
        label.setStyleSheet("""
            QLabel {
                background-color: #333;
                color: white;
                border-radius: 10px;
                padding: 6px;
            }
        """)
        label.setWordWrap(True)
        label.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Minimum)
        self.chat_vbox.insertWidget(self.chat_vbox.count() - 1, label)
        QTimer.singleShot(100, self.scroll_to_bottom)

    def add_image_bubble(self, label_text, pixmap):
        label = QLabel(label_text)
        label.setStyleSheet("font-weight: bold; padding: 4px; color: #aaa;")

        image_label = QLabel()
        image_label.setPixmap(pixmap.scaledToWidth(200))
        image_label.setStyleSheet("margin-bottom: 10px;")

        self.chat_vbox.insertWidget(self.chat_vbox.count() - 1, label)
        self.chat_vbox.insertWidget(self.chat_vbox.count() - 1, image_label)
        QTimer.singleShot(100, self.scroll_to_bottom)

    def scroll_to_bottom(self):
        self.scroll_area.verticalScrollBar().setValue(self.scroll_area.verticalScrollBar().maximum())

    def clear_chat(self):
        for i in reversed(range(self.chat_vbox.count() - 1)):
            widget = self.chat_vbox.itemAt(i).widget()
            if widget:
                widget.setParent(None)
