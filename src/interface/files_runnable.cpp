#include "interface/files_runnable.h"
#include <QTime>
#include <QFileInfo>
#include <boost/filesystem.hpp>
#include "interface/archieve_reader.h"

FilesRunnable::FilesRunnable(ngram_model<unsigned long>& m_, const QString& file_, double progress_) :
        m(m_), file(file_), progress(progress_)
{ }

void FilesRunnable::run()
{
    QFileInfo file_info(file);
    if (file_info.completeSuffix() == "txt") {
        std::string text_data = read_binary_file(file.toUtf8().constData());
        file_token(text_data, file_info.fileName());
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
            QString name = file_info.fileName() + '/' + sub_file_info.fileName();
            file_token(data, name);
        }
    }
    emit progressChanged(progress);
}

void FilesRunnable::file_token(const std::string& text, const QString& file_name)
{
    std::vector<std::string> tokenized;
    emit textChanged(QTime::currentTime().toString() + " : Start tokenizing file: " + file_name);
    tokenized = tokenize_text(text);
    m.update(tokenized);
    emit textChanged(QTime::currentTime().toString() + " : End tokenizing file: " + file_name);
};