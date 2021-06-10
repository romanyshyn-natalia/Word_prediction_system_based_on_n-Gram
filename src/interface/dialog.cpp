#include <QThread>
#include <cmath>
#include "interface/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::progress(const double& progress) {
    total_progress = total_progress + progress;
    if (total_progress >= 99) {
        ui->progressBar->setValue(100);
        ui->finishButton->setEnabled(true);
    }
    else
        ui->progressBar->setValue(std::ceil(total_progress));
}

void Dialog::files_output(const QString& text) {
    ui->textBrowser->append(text);
}


void Dialog::on_stopButton_clicked()
{
    delete this;
}

void Dialog::on_finishButton_clicked()
{
    delete this;
}