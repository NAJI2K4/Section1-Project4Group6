
import socket
import threading
from client_handler import ClientHandler

class ServerCore:
    def __init__(self, gui):
        self.gui = gui
        self.clients = []
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.bind(("127.0.0.1", 12345))
        self.server_socket.listen(5)
        threading.Thread(target=self.accept_clients, daemon=True).start()

    def accept_clients(self):
        while True:
            client_sock, client_addr = self.server_socket.accept()
            handler = ClientHandler(client_sock, client_addr, self)
            thread = threading.Thread(target=handler.run, daemon=True)
            self.clients.append({"sock": client_sock, "addr": client_addr, "handler": handler, "thread": thread, "username": "Unknown"})
            thread.start()
            self.gui.log(f"[+] Connection from {client_addr}")

    def broadcast(self, data, exclude=None):
        for c in self.clients:
            if c["sock"] != exclude:
                try:
                    c["sock"].sendall(data)
                except:
                    pass

    def disconnect_client(self, index):
        client = self.clients.pop(index)
        client["sock"].close()
        self.gui.log(f"[-] Disconnected {client['addr']}")
