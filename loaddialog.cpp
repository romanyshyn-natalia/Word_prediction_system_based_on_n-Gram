#include "loaddialog.h"
#include "ui_loaddialog.h"
#include "mainwindow.h"

LoadDialog::LoadDialog(NgramModel& m_, const size_t& n_grams_,
                       const QList<QString>& files_,  const double& size_, QWidget *parent) :
    QDialog(parent), m(m_), n_grams(n_grams_), files(files_), size(size_),
    ui(new Ui::LoadDialog)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    t = new Thread(this);
    connect(t, SIGNAL(progressChanged(int)), this, SLOT(progress(int)));
    connect(t, SIGNAL(files_out(QString)), this, SLOT(files_output(QString)));
}

LoadDialog::~LoadDialog()
{
    delete ui;
}

void LoadDialog::progress(int prog) {
    ui->progress_bar->setValue(prog);
}

void LoadDialog::files_output(QString output) {
    ui->listWidget->addItem(output);
}

void LoadDialog::on_pushButton_2_clicked()
{
    t->run(m, n_grams, files, size);
}
