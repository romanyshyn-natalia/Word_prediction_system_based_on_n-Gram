#include "interface/main_window.h"
#include "ui_main_window.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>
#include <QStringList>
#include <QStorageInfo>
#include <QDebug>
#include <QThread>
#include <QThreadPool>
#include <QDirIterator>
#include "interface/files_runnable.h"
#include "interface/dialog.h"


// #define PRINT_HINTS

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto lbm = boost::locale::localization_backend_manager::global();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));


    /* *******  PAGE 1   ******* */

    // set options for smoothing
    for (size_t i = 10; i > 0; i -= 1) {
        ui->smoothingBox->addItem(QString::number(i/10.0));
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

    // Total size view
    listModeltotal->insertColumns(0,2);
    listModeltotal->insertRows(0,1);
    listModeltotal->setData(listModeltotal->index(0,0), QStringLiteral("Total size: "));
    listModeltotal->setData(listModeltotal->index(0,1), QStringLiteral("0 Mb"));
    ui->sizeView->setModel(listModeltotal);
    ui->sizeView->horizontalHeader()->hide();
    ui->sizeView->verticalHeader()->hide();
    ui->sizeView->setShowGrid(false);
    ui->sizeView->resizeColumnsToContents();
    ui->sizeView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->sizeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setModel(res_model);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* *******  PAGE 2   ******* */

    // set image on page 2
    QPixmap pix(":/ngram.png");
    ui->image->setPixmap(pix);
    ui->image->setScaledContents(true);

    // set names of tabs
    ui->tabWidget->setTabText(0, "Main");
    ui->tabWidget->setTabText(1, "Test");

    ui->resultView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->resultView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->resultView->setSelectionBehavior(QAbstractItemView::SelectRows);

}

MainWindow::~MainWindow()
{
    delete ui;
}


/*
* Helper function. Take path to file check if file' size doesn't
* exceed free memory on computer if doesn't - add it to other files, if does - return -1.
* @param path to file.
* @return success in adding new file.
*/
int MainWindow::add_file(const QString& name)
{
    QStorageInfo storage = QStorageInfo::root();
    QFile file(name);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "Warning", "Error while opening file!");
    }

    // check if we have enough space
    files_size += file.size();
    if (storage.bytesAvailable() < files_size) {
        QMessageBox::warning(this, "Warning", "Not enough space on your computer");
        files_size -= file.size();
        return -1;
    }
    files.append(name);

    // insert new file name, its size into the table & change total size
    if(listModel->insertRow(listModel->rowCount())) {
        ui->filesView->setUpdatesEnabled(false);
        listModel->insertRow(listModel->rowCount() + 1);
        QFileInfo fileInfo(file.fileName());
        QString size = QString::number(file.size()/1024);
        QString info = fileInfo.fileName();
        listModel->setData(listModel->index(listModel->rowCount() - 1, 0), info);
        if (file.size()/1024/1024 > 1)
            listModel->setData(listModel->index(listModel->rowCount() - 1, 1),
                               QString::number(file.size()/1024/1024) + " Mb");
        else
            listModel->setData(listModel->index(listModel->rowCount() - 1, 1),
                               QString::number(file.size()/1024) + " Kb");
        ui->filesView->setUpdatesEnabled(true);
    }
    return 0;
}

/*
* Change total size of files in ui.
* @param nothing.
* @return nothing.
*/
void MainWindow::change_total_size()
{
    ui->filesView->setUpdatesEnabled(false);
    if (files_size/1024/1024 > 1)
        listModeltotal->setData(listModeltotal->index(0,1),
                                QString::number(int(files_size/1024/1024)) + "Mb");
    else
        listModeltotal->setData(listModeltotal->index(0,1),
                                QString::number(int(files_size/1024)) + "Kb");
    ui->sizeView->resizeColumnsToContents();
    ui->filesView->setUpdatesEnabled(true);

#ifdef PRINT_HINTS
    qDebug() << "Files size:" << files_size;
#endif //PRINT_HINTS
}

/*
* Add files.
* @param nothing.
* @return nothing.
*/
void MainWindow::on_addButton_clicked()
{
    QStorageInfo storage = QStorageInfo::root();
#ifdef PRINT_HINTS
    if (storage.isReadOnly())
        qDebug() << "isReadOnly:" << storage.isReadOnly();
    qDebug() << "availableSize:" << storage.bytesAvailable();
#endif //PRINT_HINTS

    // set allowed for users files and open file dialog
    QString filter = "All files (*.txt *.zip)";
    QStringList file_name = QFileDialog::getOpenFileNames(this, "Open files", QDir::homePath(), filter);

    // if the user has selected files - add their names to the list and change total size
    if (file_name.empty()) {
        return;
    }
    for (QString &el : file_name) {
        int res = add_file(el);
        if (res == -1)
            break;
    }
    change_total_size();
}

/*
* Add files from the chosen folder.
* @param nothing.
* @return nothing.
*/
void MainWindow::on_dirButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    QDirIterator it(dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        if (it.fileInfo().completeSuffix() == "txt" || it.fileInfo().completeSuffix() == "zip") {
            int res = add_file(it.filePath());
            if (res == -1)
                break;
        } else {
            continue;
        }
    }
    change_total_size();
}

/*
* Remove chosen files.
* @param nothing.
* @return nothing.
*/
void MainWindow::on_deleteButton_clicked()
{
    ui->filesView->setUpdatesEnabled(false);
    QModelIndexList indexes = ui->filesView->selectionModel()->selectedRows();
    std::sort(indexes.begin(), indexes.end());

    for(int i = indexes.count() - 1; i > -1; --i) {
        QFile file(files[indexes.at(i).row()]);
#ifdef PRINT_HINTS
        qDebug() << file.fileName();
#endif //PRINT_HINTS
        files_size -= file.size();
        files.removeAt(indexes.at(i).row());
        listModel->removeRow(indexes.at(i).row());
    }

    change_total_size();
    ui->filesView->setUpdatesEnabled(true);
}

/*
* Start tokenizing files. If no files provided - show warning window.
* Put each file into separate function and start thread pool with a number of threads chosen by the user.
* Call for dialog window that shows progress - percent of tokenized files.
* @param nothing.
* @return nothing.
*/
void MainWindow::on_startButton_clicked()
{
    // check if user choose any files
    if (files.empty()) {
        QMessageBox::warning(this, "Warning", "Choose file!");
        return;
    }
    ui->lineEdit->clear();
    auto *str = new QStringListModel();
    ui->resultView->setModel(str);

    size_t n_grams = ui->gramBox->value();
    size_t suggestions = ui->wordsBox->value();
    uint threads = ui->threadBox->value();
    double k = ui->smoothingBox->currentText().toDouble();

    m = ngram_model<unsigned long>{n_grams, suggestions, k};

    // create dialog window
    auto* dialog = new Dialog();
    dialog->setWindowTitle("Tokenizing progress");

    QThreadPool* pool = QThreadPool::globalInstance();
    pool->setMaxThreadCount(threads);

    int idx = 0;
    while (idx < files.size()) {
        QFile file(files[idx]);
        auto progress = ( file.size() * 100.0 ) / files_size;
        auto *t = new FilesRunnable(std::ref(m), std::ref(files[idx++]), progress);
        pool->start(t);
        QObject::connect(t, &FilesRunnable::progressChanged, dialog, &Dialog::progress);
        QObject::connect(t, &FilesRunnable::textChanged, dialog, &Dialog::files_output);
    }
    dialog->exec();
    pool->clear();

    ui->tableView->setUpdatesEnabled(false);
    res_model->clear();
    res_model->insertColumns(0,2);
    res_model->setHeaderData(0,Qt::Horizontal,QStringLiteral("Ngram"));
    res_model->setHeaderData(1,Qt::Horizontal,QStringLiteral("Frequency"));

    auto freq_ngrams = m.frequent_ngram(20);
    for (const auto &n: freq_ngrams) {
        QString temp_word;
        for (const auto &word: n.first.getContext()) {
            temp_word.push_back(QString::fromStdString(word) +  " ");
        }
        if(res_model->insertRow(res_model->rowCount())) {
#ifdef PRINT_HINTS
            std::cout << n.first.getToken() << " - " << n.second << std::endl;
#endif //PRINT_HINTS
            res_model->insertRow(res_model->rowCount() + 1);
            res_model->setData(res_model->index(res_model->rowCount() - 1, 0),
                               temp_word + " " + QString::fromStdString(n.first.getToken()));
            res_model->setData(res_model->index(res_model->rowCount() - 1, 1),
                               QString::number(n.second));
        }
    }
        ui->tableView->setUpdatesEnabled(true);

#ifdef PRINT_HINTS
    qDebug() << "Close dialog!";
#endif //PRINT_HINTS
}


/*
* When user edit line - print suggested words.
* @param nothing.
* @return nothing.
*/
void MainWindow::on_lineEdit_editingFinished()
{
    std::string user_input = ui->lineEdit->text().toUtf8().constData();

    // check if user input or model is empty
    if (user_input.empty() || m.is_empty()) {
        return;
    }

    std::vector<std::string> user_text_tokenized;
    std::vector<std::string> current_input = tokenize_text(user_input);
    user_text_tokenized.reserve(user_text_tokenized.size() + std::distance(current_input.begin(),
                                                                           current_input.end()));
    user_text_tokenized.insert(user_text_tokenized.end(), current_input.begin(), current_input.end());

    auto res = m.autocomplete(hashed_text(user_text_tokenized));
    QStringList result2;
    for (const auto &e: res){
        result2.append(QString::fromStdString(e));
        result.append(QString::fromStdString(e));
#ifdef PRINT_HINTS
        qDebug() << QString::fromStdString(e);
#endif //PRINT_HINTS
    }
    auto *str = new QStringListModel(result2);
    ui->resultView->setModel(str);

#ifdef PRINT_HINTS
    qDebug() << "Finished suggesting!";
#endif //PRINT_HINTS
}

/*
* When user click on suggested word add it
* to input string and return new suggestions.
* @param index of chosen word.
* @return nothing.
*/
void MainWindow::on_resultView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QString suggestion = " " + index.sibling(row, 0).data().toString();
    ui->lineEdit->setText(ui->lineEdit->text() + suggestion);
    ui->lineEdit->editingFinished();
}

