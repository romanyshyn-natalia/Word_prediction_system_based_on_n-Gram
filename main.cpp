#include "mainwindow.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon("..\\untitled\\qt.png"));
    w.setWindowTitle("N-gram");
    w.show();
    return a.exec();
}
