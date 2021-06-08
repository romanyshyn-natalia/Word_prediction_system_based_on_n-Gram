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

    QFileInfo fileInfo(file);
    if (fileInfo.completeSuffix() == "txt") {
        emit textChanged(QTime::currentTime().toString() + " : Start tokenizing file: " + fileInfo.fileName());
        std::string text_data = read_binary_file(file.toUtf8().constData());
        tokenized = tokenize_text(text_data);
        m.update(tokenized);
        emit textChanged(QTime::currentTime().toString() + " : End tokenizing file: " + fileInfo.fileName());
    } else if (fileInfo.completeSuffix() == "zip") {
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
            std::string cur_extension = boost::locale::to_lower(
                    file_name.substr(file_name.find_last_of('.')));
            std::cout << cur_extension << std::endl;
            if (cur_extension != ".txt") {
                continue;
            }
            std::string data = a.getWholeFileImpl();
            std::cout << file_name << std::endl;
            QString name = fileInfo.fileName() + '/' + QString::fromUtf8(file_name.c_str());
            emit textChanged(QTime::currentTime().toString() + " : Start tokenizing file: " + name);
            tokenized = tokenize_text(data);
            m.update(tokenized);
            emit textChanged(QTime::currentTime().toString() + " : End tokenizing file: " + name);
        }
    }
    emit progressChanged(prog);
}
