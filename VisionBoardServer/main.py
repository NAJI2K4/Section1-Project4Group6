
from PySide6.QtWidgets import QApplication
from server_gui import ServerGUI
import sys

if __name__ == "__main__":
    app = QApplication(sys.argv)
    gui = ServerGUI()
    gui.show()
    sys.exit(app.exec())
