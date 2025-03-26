/* NetworkStatusWindow.cpp */
#include "NetworkStatusWindow.h"

NetworkStatusWindow::NetworkStatusWindow(ClientManager *clientManager, QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Network Status");
    setGeometry(250, 250, 400, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    statusLabel = new QLabel("Establishing connection to server...", this);

    layout->addWidget(statusLabel);
    setLayout(layout);
}
