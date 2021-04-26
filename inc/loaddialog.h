#ifndef LOADDIALOG_H
#define LOADDIALOG_H

#include <QDialog>
#include <QThread>
#include "NGramModel.h"
#include "thread.h"

namespace Ui {
class LoadDialog;
}

class LoadDialog : public QDialog
{
    Q_OBJECT


public:
    explicit LoadDialog(NgramModel& m_, const size_t& n_grams_,
                        const QList<QString>& files_, const double& size_, QWidget *parent = nullptr);
    ~LoadDialog();
    Thread *t;
    NgramModel& m;
    const size_t& n_grams;
    const QList<QString>& files;
    const double& size;

private:
    Ui::LoadDialog *ui;

public slots:
    void progress(int);
    void files_output(QString);

private slots:
    void on_pushButton_2_clicked();
};

#endif // LOADDIALOG_H
