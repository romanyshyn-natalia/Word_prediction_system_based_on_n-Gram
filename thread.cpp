#include <QFile>
#include "thread.h"
#include "word_tokenizer.h"


Thread::Thread(QObject *parent) : QThread(parent)
{ }

void Thread::run(NgramModel& m, const size_t& n_grams,
                 const QList<QString>& files, const double& size) {

    std::vector<std::string> tokenized;
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    size_t suggestions;
    suggestions = 3;
    m = NgramModel{n_grams, suggestions};
    int progress;
    for (auto &file : files) {
        QFile file_info(file);
        emit files_out(file_info.fileName());
        std::string text_data = read_binary_file(file.toUtf8().constData());
        tokenized = tokenize_text(text_data);
        m.update(tokenized);

        progress = file_info.size() / size  * 100;
        emit progressChanged(progress);
        std::cout << "Ok!" << std::endl;
    }
    emit progressChanged(100);

    std::cout << "Here!" << std::endl;
}
