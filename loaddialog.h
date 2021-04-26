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
    explicit LoadDialog(std::vector<std::string>& tokenized_, NgramModel& m_,
                        const size_t& n_grams_, const QList<QString>& files_, QWidget *parent = nullptr);
    ~LoadDialog();
    Thread *t;
    std::vector<std::string>& tokenized;
    NgramModel& m;
    const size_t& n_grams;
    const QList<QString>& files;

private:
    Ui::LoadDialog *ui;

public slots:
    void progress(int);
private slots:
    void on_pushButton_2_clicked();
};

#endif // LOADDIALOG_H
