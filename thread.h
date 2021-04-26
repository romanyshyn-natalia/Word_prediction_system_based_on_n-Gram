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
    void run(std::vector<std::string>& tokenized, NgramModel& m, const size_t& n_grams,
             const QList<QString>& files);
signals:
    void progressChanged(int);
    void start();
};

#endif // THREAD_H
