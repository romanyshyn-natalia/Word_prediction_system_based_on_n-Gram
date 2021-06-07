#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "ngram_model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void progressChanged(int);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_editingFinished();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *listModel = new QStandardItemModel(NULL);
    QStandardItemModel *listModeltotal = new QStandardItemModel(NULL);
    QList<QString> files;
    long long files_size = 0;
    QStringList result;
    ngram_model<unsigned long> m;
};
#endif // MAINWINDOW_H
