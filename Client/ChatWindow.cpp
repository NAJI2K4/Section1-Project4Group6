/* ChatWindow.cpp */
#include "ChatWindow.h"
#include <QDebug>

ChatWindow::ChatWindow(ClientManager *clientManager, QWidget *parent)
    : QWidget(parent), clientManager(clientManager) {
    setWindowTitle("Chat");
    setGeometry(200, 200, 500, 400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    chatDisplay = new QTextEdit(this);
    chatDisplay->setReadOnly(true);

    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send", this);

    connect(sendButton, &QPushButton::clicked, this, &ChatWindow::sendMessage);

    mainLayout->addWidget(chatDisplay);
    mainLayout->addWidget(messageInput);
    mainLayout->addWidget(sendButton);

    setLayout(mainLayout);
}

void ChatWindow::sendMessage() {
    QString message = messageInput->text();
    if (!message.isEmpty()) {
        clientManager->sendMessage(message);
        chatDisplay->append("Me: " + message);
        messageInput->clear();
    }
}
