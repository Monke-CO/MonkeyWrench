#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/monkey-holding-wrench.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
