#include <QtWidgets/QApplication>

#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    LoginWindow *start = new LoginWindow();
    start->show();

    return app.exec();
}
