#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "loginwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showMainApplication();  // Show main UI after login

private:
    LoginWidget *loginWidget;
    QWidget *mainWidget;
    void initMainUI();
};

#endif // MAINWINDOW_H
