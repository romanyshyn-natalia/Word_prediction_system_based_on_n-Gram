#include "interface/files_runnable.h"
#include <QTime>
#include <QFileInfo>
#include <boost/filesystem.hpp>
#include "interface/archieve_reader.h"

FilesRunnable::FilesRunnable(ngram_model<unsigned long>& m_, const QString& file_, double prog_) :
        m(m_), file(file_), prog(prog_)
{ }

void FilesRunnable::run()
{
    std::vector<std::string> tokenized;
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    QFileInfo file_info(file);
    if (file_info.completeSuffix() == "txt") {
        emit textChanged(QTime::currentTime().toString() + " : Start tokenizing file: " + file_info.fileName());
        std::string text_data = read_binary_file(file.toUtf8().constData());
        tokenized = tokenize_text(text_data);
        m.update(tokenized);
        emit textChanged(QTime::currentTime().toString() + " : End tokenizing file: " + file_info.fileName());
    } else if (file_info.completeSuffix() == "zip") {
        std::ifstream raw_file(file.toStdString(), std::ios::binary);
        auto buffer = [&raw_file] {
            std::ostringstream ss{};
            ss << raw_file.rdbuf();
            return ss.str();
        }();
        Archive a;
        a.initFromMemoryImpl(buffer);
        while (a.nextFileImpl()) {
            auto file_name = a.getFileNameImpl();
            QFileInfo sub_file_info(QString::fromUtf8(file_name.c_str()));
            if (sub_file_info.completeSuffix() != "txt") {
                continue;
            }
            std::string data = a.getWholeFileImpl();
            std::cout << file_name << std::endl;
            QString name = file_info.fileName() + '/' + sub_file_info.fileName();
            emit textChanged(QTime::currentTime().toString() + " : Start tokenizing file: " + name);
            tokenized = tokenize_text(data);
            m.update(tokenized);
            emit textChanged(QTime::currentTime().toString() + " : End tokenizing file: " + name);
        }
    }
    emit progressChanged(prog);
}
