/* ClientApp.cpp */
#include "ClientApp.h"
#include "NetworkStatusWindow.h"
#include "LoginWindow.h"
#include "ChatWindow.h"

void ClientApp::run(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ClientManager clientManager;
    NetworkStatusWindow networkWindow(&clientManager);
    networkWindow.show();
    app.processEvents(); // Ensure UI updates

    LoginWindow loginWindow(&clientManager);
    loginWindow.show();

    app.exec();
}
