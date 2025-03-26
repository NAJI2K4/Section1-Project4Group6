#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class ClientManager;  // Forward declaration

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(ClientManager* manager, QWidget *parent = nullptr);

signals:
    void loginRequested(const QString &username, const QString &password);

private slots:
    void handleLogin();

private:
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    ClientManager* clientManager;
};

#endif // LOGINWINDOW_H
