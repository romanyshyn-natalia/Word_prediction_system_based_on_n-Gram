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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void progressChanged(int);

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_startButton_clicked();

    void on_lineEdit_editingFinished();

    void on_resultView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *listModel = new QStandardItemModel();
    QStandardItemModel *listModeltotal = new QStandardItemModel();
    QList<QString> files;
    long long files_size = 0;
    QStringList result;
    ngram_model<unsigned long> m;
};
#endif // MAINWINDOW_H
