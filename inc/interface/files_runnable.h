#ifndef FILES_RUNNABLE_H
#define FILES_RUNNABLE_H
#include <QRunnable>
#include <QDebug>
#include <QThread>
#include "model/ngram_model.h"
#include "model/word_tokenizer.h"

class FilesRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    FilesRunnable(ngram_model<unsigned long>& m_, const QString& file_, double progress_);
    void run() override;
    void file_token(const std::string& text, const QString& file_name);

signals:
    void progressChanged(double);
    void textChanged(QString);

private:
    ngram_model<unsigned long>& m;
    const QString& file;
    double progress;
};

#endif // FILES_RUNNABLE_H
