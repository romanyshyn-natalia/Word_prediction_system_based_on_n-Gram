#ifndef FILESRUNNABLE_H
#define FILESRUNNABLE_H
#include <QRunnable>
#include <QDebug>
#include <QThread>
#include "ngram_model.h"
#include "word_tokenizer.h"

class FilesRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    FilesRunnable(ngram_model<unsigned long>& m_, const QString& file_, double prog_);
    void run() override;

signals:
    void progressChanged(double);
    void textChanged(QString);

private:
    ngram_model<unsigned long>& m;
    const QString& file;
    double prog;
};

#endif // FILESRUNNABLE_H
