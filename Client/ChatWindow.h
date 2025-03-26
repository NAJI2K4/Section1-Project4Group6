/* ChatWindow.h */
#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "ClientManager.h"

class ChatWindow : public QWidget {
    Q_OBJECT

public:
    explicit ChatWindow(ClientManager *clientManager, QWidget *parent = nullptr);

private slots:
    void sendMessage();

private:
    ClientManager *clientManager;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
};

#endif // CHATWINDOW_H
