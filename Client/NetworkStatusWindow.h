/* NetworkStatusWindow.h */
#ifndef NETWORKSTATUSWINDOW_H
#define NETWORKSTATUSWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "ClientManager.h"

class NetworkStatusWindow : public QWidget {
    Q_OBJECT

public:
    explicit NetworkStatusWindow(ClientManager *clientManager, QWidget *parent = nullptr);

private:
    QLabel *statusLabel;
};

#endif // NETWORKSTATUSWINDOW_H
