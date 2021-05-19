#ifndef FILESRUNNABLE_H
#define FILESRUNNABLE_H
#include <QRunnable>
#include <QDebug>
#include <QThread>
#include "ngrammodel.h"
#include "word_tokenizer.h"

class FilesRunnable : public QObject, public QRunnable
{
    Q_OBJECT
public:
    FilesRunnable(NgramModel& m_, const QString& file_, double prog_);
    void run() override;

signals:
    void progressChanged(double);
    void textChanged(QString);

private:
    NgramModel& m;
    const QString& file;
    double prog;
};

#endif // FILESRUNNABLE_H
