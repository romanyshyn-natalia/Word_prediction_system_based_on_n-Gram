#include "../inc/filesrunnable.h"
#include <QTime>

FilesRunnable::FilesRunnable(NgramModel& m_, const QString& file_, double prog_) :
        m(m_), file(file_), prog(prog_)
{ }

void FilesRunnable::run()
{
    qDebug() << "Hello world from thread" << QThread::currentThread();
    qDebug() << QTime::currentTime();

    std::vector<std::string> tokenized;
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    emit textChanged(QTime::currentTime().toString() + " : Start tokenizing file: " + file.split('/').back());
    std::string text_data = read_binary_file(file.toUtf8().constData());
    tokenized = tokenize_text(text_data);
    m.update(tokenized);
    emit textChanged(QTime::currentTime().toString() + " : End tokenizing file: " + file.split('/').back());
    emit progressChanged(prog);
}
