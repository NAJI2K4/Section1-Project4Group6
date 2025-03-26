/*
#include "LoginWindow.h"
#include "ClientManager.h"
#include <QDebug>

LoginWindow::LoginWindow(ClientManager* manager, QWidget *parent)
    : QWidget(parent), clientManager(manager) {
    setWindowTitle("Login");

    QVBoxLayout *layout = new QVBoxLayout(this);

    usernameInput = new QLineEdit(this);
    usernameInput->setPlaceholderText("Username");

    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    layout->addWidget(usernameInput);
    layout->addWidget(passwordInput);
    layout->addWidget(loginButton);

    setLayout(layout);
}

void LoginWindow::handleLogin() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (!username.isEmpty() && !password.isEmpty()) {
        emit loginRequested(username, password);
        clientManager->login(username, password);
    } else {
        qDebug() << "Login fields cannot be empty.";
    }
}
*/
#include "LoginWindow.h"
#include "ClientManager.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>

LoginWindow::LoginWindow(ClientManager* manager, QWidget *parent)
    : QWidget(parent), clientManager(manager) {
    setWindowTitle("Login");
    setGeometry(600, 400, 800, 800);

    // === UI Setup ===
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *headingLabel = new QLabel("Sign in to VisionBoard", this);
    headingLabel->setAlignment(Qt::AlignCenter);
    headingLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    QLabel *unameLabel = new QLabel("Username: ", this);
    QLabel *passLabel = new QLabel("Password: ", this);

    usernameInput = new QLineEdit(this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::handleLogin);

    QFrame *loginFrame = new QFrame(this);
    QVBoxLayout *frameLayout = new QVBoxLayout(loginFrame);
    frameLayout->setSpacing(10);
    frameLayout->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(unameLabel);
    frameLayout->addWidget(usernameInput);
    frameLayout->addWidget(passLabel);
    frameLayout->addWidget(passwordInput);
    frameLayout->addWidget(loginButton);
    loginFrame->setLayout(frameLayout);
    loginFrame->setFixedSize(300, 200);

    QHBoxLayout *centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(loginFrame);
    centerLayout->addStretch();

    mainLayout->addStretch();
    mainLayout->addWidget(headingLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // === Styling ===
    setStyleSheet(R"(
        QWidget {
            background-color: hsl(227, 59%, 21%);
        }
        QFrame {
            background-color: hsl(227, 59%, 19%);
            border-radius: 10px;
        }
        QLabel {
            color: White;
        }
        QLineEdit {
            color: White;
            background-color: hsl(0, 1%, 15%);
            border-radius: 5px;
            border: solid black;
            padding: 5px;
        }
        QPushButton {
            background-color: hsl(200, 60%, 40%);
            border-radius: 5px;
            color: white;
            padding: 5px;
        }
    )");
}

void LoginWindow::handleLogin() {
    QString username = usernameInput->text();
    QString password = passwordInput->text();

    if (!username.isEmpty() && !password.isEmpty()) {
        emit loginRequested(username, password);
        clientManager->login(username, password);
    } else {
        qDebug() << "Login fields cannot be empty.";
    }
}
