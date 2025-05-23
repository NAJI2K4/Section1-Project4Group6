#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateLog(const QString& message);
    void updateClientList(const QStringList& usernames);

private:
    Ui::MainWindow* ui;
    Server* server;
};

#endif // MAINWINDOW_H
