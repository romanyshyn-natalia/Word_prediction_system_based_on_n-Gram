#include "main_window.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QFont f = a.font();
//    f.setFamily("Monaco");
//    f.setPointSize(22);
//    a.setFont(f);
    MainWindow w;
    QApplication::setWindowIcon(QIcon(":/qt.png"));
    w.setWindowTitle("N-gram");
    w.show();
    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
    return QApplication::exec();
}
