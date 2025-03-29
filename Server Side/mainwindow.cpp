
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , server(new Server(this))
{
    ui->setupUi(this);

    connect(ui->startServer, &QPushButton::clicked, this, [this]() {
        server->startServer(12345);
        updateLog("Server started on port 12345");
        });

    connect(ui->stopServer, &QPushButton::clicked, this, [this]() {
        server->stopServer();
        updateLog("Server stopped");
        });

    connect(server, &Server::logMessage, this, &MainWindow::updateLog);
    connect(server, &Server::clientListUpdated, this, &MainWindow::updateClientList);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateLog(const QString& message) {
    if (ui && ui->clientLogs) {
        ui->clientLogs->append(QDateTime::currentDateTime().toString("[hh:mm:ss] ") + message);
    }
}

void MainWindow::updateClientList(const QStringList& usernames) {
    try {
        ui->displaysClientsList->setUpdatesEnabled(false);

        ui->displaysClientsList->clear();

        for (const QString& username : usernames) {
            ui->displaysClientsList->addItem(username);
        }

        if (ui->clientCountLabel) {
            ui->clientCountLabel->setText("Connected Clients: " + QString::number(usernames.size()));
        }

        ui->displaysClientsList->setUpdatesEnabled(true); 
    }
    catch (const std::exception& ex) {
        qDebug() << "Exception in updateClientList:" << ex.what();
        updateLog("⚠️ UI update failed: " + QString::fromStdString(ex.what()));
    }
    catch (...) {
        qDebug() << "Unknown exception in updateClientList";
        updateLog("⚠️ UI update failed due to unknown error.");
    }
}