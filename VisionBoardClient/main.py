#print("🐍 Top-level code executing...")  # Should always run

import sys

try:
    from PySide6.QtWidgets import QApplication
    from network_handler import NetworkHandler
    from login_window import LoginWindow
    #print("✅ All imports successful.")
except Exception as e:
    print(f"❌ Import failed: {e}")

def main():
    #print("🚀 Starting main()...")

    try:
        app = QApplication(sys.argv)
        #print("🛠️ QApplication created.")
        
        network = NetworkHandler()
        #print("🌐 NetworkHandler initialized.")
        
        network.connect_to_server()
        #print("🔌 connect_to_server() called.")

        login_window = LoginWindow(network)
        login_window.show()
       # print("👁️ Login window shown.")

        sys.exit(app.exec())
    except Exception as e:
        print(f"🔥 Crash inside main(): {e}")

if __name__ == "__main__":
    #print("📌 __name__ is __main__")
    main()
