#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QIcon appIcon("://icons/16x16/globe-icon.png");
//    w.setWindowIcon(appIcon);
    a.setWindowIcon(appIcon);
    w.show();

    return a.exec();
}
