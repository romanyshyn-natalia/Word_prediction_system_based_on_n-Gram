#include "../inc/mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>
#include <QStringList>
#include <QStorageInfo>
#include <QDebug>
#include <QCompleter>
#include <QThread>
#include <QThreadPool>
#include "../inc/word_tokenizer.h"
#include "../inc/filesrunnable.h"
#include "../inc/dialog.h"

#include <QStandardItemModel>
QStandardItemModel *listModel = new QStandardItemModel(NULL);
QStandardItemModel *listModeltotal = new QStandardItemModel(NULL);
QList<QString> files;
long long files_size = 0;
QStringList result;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* *******  PAGE 1   ******* */

    // set options for smoothing
    for (double i = 0.1; i < 1; i += 0.1) {
        ui->smoothingBox->addItem(QString::number(i));
    }

    // set minimum n-gram and suggested words
    ui->gramBox->setMinimum(2);
    ui->wordsBox->setMinimum(3);

    // set min/max threads
    ui->threadBox->setMinimum(1);
    ui->threadBox->setMaximum(8);

    // Files table view
    listModel->insertColumns(0,2);
    listModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("File name"));
    listModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("Size"));
    ui->filesView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->filesView->setModel(listModel);
    ui->filesView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->filesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->filesView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->filesView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Total size view
    listModeltotal->insertColumns(0,2);
    listModeltotal->insertRows(0,1);
    listModeltotal->setData(listModeltotal->index(0,0), QStringLiteral("Total size: "));
    listModeltotal->setData(listModeltotal->index(0,1), QStringLiteral("0 Mb"));
    ui->tableView_2->setModel(listModeltotal);
    ui->tableView_2->horizontalHeader()->hide();
    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_2->setShowGrid(false);
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_2->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* *******  PAGE 2   ******* */

    // set image on page 2
    QPixmap pix("../img/ngram.png");
    ui->label_4->setPixmap(pix);
    ui->label_4->setScaledContents(true);

    // Page 2 set line for user input with auto complete
    ui->tabWidget->setTabText(0, "Main");
    ui->tabWidget->setTabText(1, "Test");

    QCompleter *completer = new QCompleter(result, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(completer);
    result << "I love";

    // ui->listView->setModel(str);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Add files */
void MainWindow::on_pushButton_clicked()
{
    QStorageInfo storage = QStorageInfo::root();
    if (storage.isReadOnly())
        qDebug() << "isReadOnly:" << storage.isReadOnly();
    qDebug() << "availableSize:" << storage.bytesAvailable();

    QString filter = "All files (*.txt)";
    QStringList file_name = QFileDialog::getOpenFileNames(this, "Choose files", QDir::homePath(), filter);

    if (file_name.empty()) {
        return;
    }
    for (QString &el : file_name) {
        QFile file(el);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::warning(this, "Warning", "Error while opening file!");
        }

        // check if we have enough space
        files_size += file.size();
        if (storage.bytesAvailable() < files_size) {
            QMessageBox::warning(this, "Warning", "Not enough space on your computer");
            files_size -= file.size();
            break;
        }
        files.append(el);

        if(listModel->insertRow(listModel->rowCount())) {
            ui->filesView->setUpdatesEnabled(false);
            listModel->insertRow(listModel->rowCount() + 1);
            QFileInfo fileInfo(file.fileName());
            QString size = QString::number(file.size()/1024);
            QString info = fileInfo.fileName();
            listModel->setData(listModel->index(listModel->rowCount() - 1, 0), info);
            listModel->setData(listModel->index(listModel->rowCount() - 1, 1), size + " Kb");
            ui->filesView->setUpdatesEnabled(true);
        }
    }

    ui->filesView->setUpdatesEnabled(false);
    QString sizes = QString::number(int(files_size/1024));
    listModeltotal->setData(listModeltotal->index(0,1), sizes + "Kb");
    ui->tableView_2->resizeColumnsToContents();
    ui->filesView->setUpdatesEnabled(true);

    qDebug() << files[0].size()/1024;
    qDebug() << "Files size" << files_size;
}

/* Remove files */
void MainWindow::on_pushButton_3_clicked()
{
    ui->filesView->setUpdatesEnabled(false);
    QModelIndexList indexes = ui->filesView->selectionModel()->selectedRows();
    qSort(indexes.begin(), indexes.end());

    for(int i = indexes.count() - 1; i > -1; --i) {
        QFile file(files[indexes.at(i).row()]);
        files_size -= file.size();
        files.removeAt(indexes.at(i).row());
        listModel->removeRow(indexes.at(i).row());
    }

    ui->filesView->setUpdatesEnabled(true);

    ui->filesView->setUpdatesEnabled(false);
    QString sizes = QString::number(int(files_size/1024));
    listModeltotal->setData(listModeltotal->index(0,1), sizes + "Kb");
    ui->tableView_2->resizeColumnsToContents();
    ui->filesView->setUpdatesEnabled(true);
}


NgramModel m;
void MainWindow::on_pushButton_2_clicked()
{
    // check if user choose any files
    if (files.empty()) {
        QMessageBox::warning(this, "Warning", "Choose file!");
        return;
    }

    size_t n_grams = ui->gramBox->value();
    size_t suggestions = ui->wordsBox->value();
    uint threads = ui->threadBox->value();
    double k = ui->smoothingBox->currentText().toDouble();

    m = NgramModel{n_grams, suggestions, k};

    // create dialog window
    auto* dialog = new Dialog();

    QThreadPool* pool = QThreadPool::globalInstance();
    pool->setMaxThreadCount(threads);

    int idx = 0;
    while (idx < files.size()) {
        QFile file(files[idx]);
        double prog = ( file.size() * 100.0 ) / files_size;
        auto *t = new FilesRunnable(std::ref(m), std::ref(files[idx++]), prog);
        pool->start(t);
        QObject::connect(t, &FilesRunnable::progressChanged, dialog, &Dialog::progress);
        QObject::connect(t, &FilesRunnable::textChanged, dialog, &Dialog::files_output);
    }
    dialog->exec();
    pool->clear();

    qDebug() << "Here!";

}


void MainWindow::on_lineEdit_editingFinished()
{
    std::string user_input = ui->lineEdit->text().toUtf8().constData();

    // check model
    if (user_input.empty()) {
        return;
    }

    std::vector<std::string> user_text_tokenized;
    std::vector<std::string> current_input = tokenize_text(user_input);
    user_text_tokenized.reserve(user_text_tokenized.size() + std::distance(current_input.begin(),
                                                                           current_input.end()));
    user_text_tokenized.insert(user_text_tokenized.end(), current_input.begin(), current_input.end());

    auto res = m.autocomplete(user_text_tokenized);
    QStringList result2;
    for (const auto &e: res){
        result2.append(QString::fromStdString(e));
        result.append(QString::fromStdString(e));
        qDebug() << QString::fromStdString(e);
        std::cout << e << std::endl;
    }
    QStringListModel *str = new QStringListModel(result2);
    ui->listView->setModel(str);
    qDebug() << "Finished!";
}

