#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon("../qt.png"));
    w.setWindowTitle("N-gram");
    w.show();
    return a.exec();
}
