#include "gowindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GoWindow w;
    w.show();

    return a.exec();
}
