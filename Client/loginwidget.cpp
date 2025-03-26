/*
#include "loginwidget.h"
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void LoginWidget::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Labels
    headingLabel = new QLabel("Sign in to VisionBoard", this);
    unameLabel = new QLabel("Username: ", this);
    passLabel = new QLabel("Password: ", this);

    // Input fields
    unameEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);
    passEdit->setEchoMode(QLineEdit::Password);

    // Button
    signinButton = new QPushButton("Sign in", this);
    connect(signinButton, &QPushButton::clicked, this, &LoginWidget::handleLogin);

    // Layout frame
    QFrame *loginFrame = new QFrame(this);
    QVBoxLayout *frameLayout = new QVBoxLayout(loginFrame);
    frameLayout->setSpacing(10);
    frameLayout->addWidget(unameLabel);
    frameLayout->addWidget(unameEdit);
    frameLayout->addWidget(passLabel);
    frameLayout->addWidget(passEdit);
    frameLayout->addWidget(signinButton);

    loginFrame->setLayout(frameLayout);
    loginFrame->setFixedSize(450, 200);

    // Add to main layout
    mainLayout->addWidget(headingLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(loginFrame, 0, Qt::AlignCenter);

    setLayout(mainLayout);

    // Styling
    setStyleSheet(R"(
        QWidget {
            background-color: hsl(227, 59%, 21%);
        }
        QFrame {
            background-color: hsl(227, 59%, 19%);
        }
        QLabel {
            color: White;
        }
        QLineEdit {
            color: White;
            background-color: hsl(0, 1%, 15%);
            border-radius: 5px;
            border: solid black;
        }
        QPushButton {
            background-color: hsl(200, 60%, 40%);
            border-radius: 5px;
            color: white;
        }
    )");
}

void LoginWidget::handleLogin() {
    QString username = unameEdit->text();
    QString password = passEdit->text();

    if (!username.isEmpty() && !password.isEmpty()) {
        emit loginRequested(username, password);
        emit loginSuccess();  // Optional: triggers switch to main window
    } else {
        qDebug() << "Login fields cannot be empty.";
    }
}
*/
#include "loginwidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    initUI();
}

void LoginWidget::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *headingLabel = new QLabel("Sign in to VisionBoard", this);
    headingLabel->setAlignment(Qt::AlignCenter);
    headingLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    unameLabel = new QLabel("Username: ", this);
    passLabel = new QLabel("Password: ", this);

    unameEdit = new QLineEdit(this);
    passEdit = new QLineEdit(this);
    passEdit->setEchoMode(QLineEdit::Password);

    signinButton = new QPushButton("Sign in", this);
    connect(signinButton, &QPushButton::clicked, this, &LoginWidget::handleLogin);

    // Frame layout for login
    QFrame *loginFrame = new QFrame(this);
    QVBoxLayout *frameLayout = new QVBoxLayout(loginFrame);
    frameLayout->setSpacing(10);
    frameLayout->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(unameLabel);
    frameLayout->addWidget(unameEdit);
    frameLayout->addWidget(passLabel);
    frameLayout->addWidget(passEdit);
    frameLayout->addWidget(signinButton);

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

    // Styling
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

void LoginWidget::handleLogin() {
    QString username = unameEdit->text();
    QString password = passEdit->text();

    if (!username.isEmpty() && !password.isEmpty()) {
        emit loginRequested(username, password);
        emit loginSuccess();  // optional
    } else {
        qDebug() << "Login fields cannot be empty.";
    }
}

