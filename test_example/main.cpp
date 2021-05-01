//#include "mainwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include "QSqlError"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgres");
    db.setPassword("postgres");
    db.setUserName("postgres");
    if(db.open())
    {
        qDebug() <<"opened" ;
        QSqlQuery query("SELECT * FROM university");
            while (query.next()) {
//                QString country = query.value(0).toString();
                qDebug() << query.value(0).toString() + "  " + query.value(1).toString() + "  " + query.value(2).toString() + "  " + query.value(3).toString() + "  " + query.value(4).toString();
            }
        db.close();
    } else {
        qDebug() << db.lastError().text();
        exit(1);
    }

    return app.exec();
}
