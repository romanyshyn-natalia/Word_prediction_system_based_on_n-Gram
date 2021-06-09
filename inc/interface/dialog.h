#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFuture>
#include <QFutureWatcher>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog() override;


public slots:
    void progress(const double&);
    void files_output(const QString&);

signals:
    void stop_all();

private slots:
    void on_stopButton_clicked();

    void on_finishButton_clicked();

private:
    Ui::Dialog *ui;
    double total_progress = 0;
};

#endif // DIALOG_H
