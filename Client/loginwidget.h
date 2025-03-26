#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginRequested(const QString &username, const QString &password);
    void loginSuccess();  // Optional: useful for switching UI after login

private slots:
    void handleLogin();

private:
    void initUI();

    QLabel *headingLabel;
    QLabel *unameLabel;
    QLabel *passLabel;

    QLineEdit *unameEdit;
    QLineEdit *passEdit;

    QPushButton *signinButton;
};

#endif // LOGINWIDGET_H
