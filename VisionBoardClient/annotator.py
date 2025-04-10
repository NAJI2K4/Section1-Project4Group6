from PySide6.QtWidgets import QLabel
from PySide6.QtGui import QPixmap, QPainter, QPen, QColor
from PySide6.QtCore import Qt, QByteArray, QBuffer, QIODevice

class Annotator(QLabel):
    def __init__(self):
        super().__init__()
        self.setFixedSize(300, 300)
        self.setStyleSheet("background-color: white;")
        self.image = QPixmap(self.size())
        self.image.fill(Qt.white)
        self.setPixmap(self.image)
        self.last_point = None

    def load_image(self, path):
        self.image = QPixmap(path).scaled(self.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation)
        self.setPixmap(self.image)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.last_point = event.position().toPoint()

    def mouseMoveEvent(self, event):
        if event.buttons() & Qt.LeftButton and self.last_point:
            painter = QPainter(self.image)
            pen = QPen(QColor("red"), 3)
            painter.setPen(pen)
            painter.drawLine(self.last_point, event.position().toPoint())
            painter.end()
            self.setPixmap(self.image)
            self.last_point = event.position().toPoint()

    """def get_image_bytes(self):
        ba = QByteArray()
        buffer = QBuffer(ba)
        buffer.open(QIODevice.WriteOnly)
        self.image.save(buffer, "PNG")
        return ba.data()"""
    
    def get_image_bytes(self):
    # Resize before sending to avoid huge payloads
        max_size = 600  # You can increase this for higher resolution
        scaled = self.image.scaled(max_size, max_size, Qt.KeepAspectRatio, Qt.SmoothTransformation)
        ba = QByteArray()
        buffer = QBuffer(ba)
        buffer.open(QIODevice.WriteOnly)
        scaled.save(buffer, "PNG")
        return ba.data()
