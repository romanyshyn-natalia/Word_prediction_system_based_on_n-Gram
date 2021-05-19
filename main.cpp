#include "inc/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont f = a.font();
    f.setFamily("Monaco");
    f.setPointSize(22);
    a.setFont(f);
    MainWindow w;
    a.setWindowIcon(QIcon("../img/qt.png"));
    w.setWindowTitle("N-gram");
    w.show();
    return a.exec();
}
