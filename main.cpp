#include "interface/main_window.h"
#include <QApplication>
#include <QFile>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>
#include "QSqlError"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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
