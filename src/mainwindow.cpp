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
#include "../inc/word_tokenizer.h"

#include <QStandardItemModel>
QStandardItemModel *listModel = new QStandardItemModel(NULL);
QStandardItemModel *listModeltotal = new QStandardItemModel(NULL);
QList<QString> files;
double files_size = 0;
// QStringListModel *str = new QStringListModel(NULL);
QStringList result;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* *******  PAGE 1   ******* */

    // set options for smoothing
    ui->comboBox->addItem("None");
    ui->comboBox->addItem("Add-k");
    ui->comboBox->addItem("Laplace");
    ui->spinBox_2->setMinimum(2);

    // Files table view
    listModel->insertColumns(0,2);
    listModel->setHeaderData(0,Qt::Horizontal,QStringLiteral("File name"));
    listModel->setHeaderData(1,Qt::Horizontal,QStringLiteral("Size"));
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_3->setModel(listModel);
    ui->tableView_3->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_3->setSelectionMode(QAbstractItemView::SingleSelection);

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
            ui->tableView_3->setUpdatesEnabled(false);
            listModel->insertRow(listModel->rowCount() + 1);
            QFileInfo fileInfo(file.fileName());
            QString size = QString::number(file.size()/1024/1024);
            QString info = fileInfo.fileName();
            listModel->setData(listModel->index(listModel->rowCount() - 1, 0), info);
            listModel->setData(listModel->index(listModel->rowCount() - 1, 1), size + " Mb");
            ui->tableView_3->setUpdatesEnabled(true);
        }
    }

    ui->tableView_3->setUpdatesEnabled(false);
    QString sizes = QString::number(int(files_size/1024/1024));
    listModeltotal->setData(listModeltotal->index(0,1), sizes + "Mb");
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_3->setUpdatesEnabled(true);

    qDebug() << files[0].size()/1024/1024;
    qDebug() << "Files size" << files_size;
}

/* Remove files */
void MainWindow::on_pushButton_3_clicked()
{
    ui->tableView_3->setUpdatesEnabled(false);
    QModelIndexList indexes = ui->tableView_3->selectionModel()->selectedRows();
    // QModelIndexList indexes = ui->tableView_3->selectionModel()->selectedIndexes();
    qSort(indexes.begin(), indexes.end());

    for(int i = indexes.count() - 1; i > -1; --i) {
        QFile file(files[indexes.at(i).row()]);
        files_size -= file.size();
        files.removeAt(indexes.at(i).row());
        listModel->removeRow(indexes.at(i).row());
    }

    ui->tableView_3->setUpdatesEnabled(true);

    ui->tableView_3->setUpdatesEnabled(false);
    QString sizes = QString::number(int(files_size/1024/1024));
    listModeltotal->setData(listModeltotal->index(0,1), sizes + "Mb");
    ui->tableView_2->resizeColumnsToContents();
    ui->tableView_3->setUpdatesEnabled(true);
}

/* combo box change option */
void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    qDebug() << arg1;
    qDebug() << ui->comboBox->currentText();
}


NgramModel m;
void MainWindow::on_pushButton_2_clicked()
{
    // check if user choose any files
    if (files.empty()) {
        QMessageBox::warning(this, "Warning", "Choose file!");
        return;
    }

    // open dialog window
    size_t n_grams = ui->spinBox_2->value();
    auto* dialog = new LoadDialog(std::ref(m), std::ref(n_grams),
                                  std::ref(files), std::ref(files_size));
    dialog->exec();



// добавити потік
//    auto lbm = boost::locale::localization_backend_manager::global();
//    auto s = lbm.get_all_backends();
//    lbm.select("icu");
//    boost::locale::localization_backend_manager::global(lbm);
//    boost::locale::generator g;
//    std::locale::global(g(""));
//
//    size_t n_grams, suggestions;
//    n_grams = ui->spinBox_2->value();
//    suggestions = 3;
//    m = NgramModel{n_grams, suggestions};
//    for (auto &file : files) {
//        std::string text_data = read_binary_file(file.toUtf8().constData());
//        tokenized = tokenize_text(text_data);
//        m.update(tokenized);
//        qDebug() << "Ok!";
//    }

    qDebug() << "Here!";

}


void MainWindow::on_lineEdit_editingFinished()
{
//    size_t n_grams, suggestions;
//    n_grams = ui->spinBox_2->value();
//    std::cout << n_grams << std::endl;
//    // n_grams = ui->comboBox->currentText().toInt();
//    suggestions = 3;
//    n_grams = 1;
//    NgramModel m{n_grams, suggestions};
//    m.update(tokenized);

    std::vector<std::string> user_text_tokenized;

    std::string user_input = ui->lineEdit->text().toUtf8().constData();

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

void MainWindow::on_spinBox_2_textChanged(const QString &arg1)
{
    ui->spinBox_3->setMinimum(arg1.toInt());
}
