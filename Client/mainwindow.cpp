#include "mainwindow.h"
#include "loginwidget.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("VisionBoard - Login");
    resize(800, 600);

    loginWidget = new LoginWidget(this);
    setCentralWidget(loginWidget);

    // Connect login success signal to main UI display
    connect(loginWidget, &LoginWidget::loginSuccess, this, &MainWindow::showMainApplication);
}

void MainWindow::showMainApplication() {
    qDebug() << "Login successful! Switching to main application UI.";

    initMainUI(); // create the main app UI

    setCentralWidget(mainWidget); // swap login UI with main UI
    setWindowTitle("VisionBoard - Main App");
}

void MainWindow::initMainUI() {
    mainWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mainWidget);

    QLabel *welcomeLabel = new QLabel("Welcome to the VisionBoard!", mainWidget);
    layout->addWidget(welcomeLabel);

    mainWidget->setLayout(layout);
}
