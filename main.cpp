#include "mainwindow.h"
#include <QApplication>
QTime tm;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    tm.start();
    return a.exec();
}
