#ifndef THREAD_H
#define THREAD_H


#include <QThread>
#include "NGramModel.h"

class Thread : public QThread
{
    Q_OBJECT
public:
    // explicit Thread(QObject *parent = nullptr, bool b = false);
    explicit Thread(QObject *parent = nullptr);
    // ~Thread();

    bool Stop;
public slots:
    void run(NgramModel& m, const size_t& n_grams,
             const QList<QString>& files, const double& size);
signals:
    void progressChanged(int);
    void files_out(QString);
};

#endif // THREAD_H
