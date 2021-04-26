#include "inc/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon("../img/qt.png"));
    w.setWindowTitle("N-gram");
    w.show();
    return a.exec();
}
