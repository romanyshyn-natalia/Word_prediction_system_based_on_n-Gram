#include "loaddialog.h"
#include "ui_loaddialog.h"
#include "mainwindow.h"

LoadDialog::LoadDialog(std::vector<std::string>& tokenized_, NgramModel& m_,
                       const size_t& n_grams_, const QList<QString>& files_, QWidget *parent) :
    QDialog(parent), tokenized(tokenized_), m(m_), n_grams(n_grams_), files(files_),
    ui(new Ui::LoadDialog)
{
    ui->setupUi(this);
    ui->pushButton->setEnabled(false);
    t = new Thread(this);
    connect(t, SIGNAL(progressChanged(int)), this, SLOT(progress(int)));
}

LoadDialog::~LoadDialog()
{
    delete ui;
}

void LoadDialog::progress(int prog) {
    ui->progress_bar->setValue(prog);
}

void LoadDialog::on_pushButton_2_clicked()
{
    t->run(tokenized, m, n_grams, files);
}
