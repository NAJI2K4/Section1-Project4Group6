/* ClientApp.h */
#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include <QApplication>
#include "NetworkStatusWindow.h"
#include "LoginWindow.h"
#include "ChatWindow.h"

class ClientApp {
public:
    static void run(int argc, char *argv[]);
};

#endif // CLIENTAPP_H
