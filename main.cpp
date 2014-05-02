#include "mainwindow.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    QtMessageHandler * handler =  new QtMessageHandler();
    qInstallMessageHandler(* handler);
    return a.exec();
}
