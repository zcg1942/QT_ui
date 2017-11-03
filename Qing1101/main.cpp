#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("lena 秀秀");
   // w.resize(325, 120);
    w.show();

    return a.exec();
}
