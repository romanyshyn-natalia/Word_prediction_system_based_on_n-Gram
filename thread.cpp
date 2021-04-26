#include "thread.h"
#include "word_tokenizer.h"


Thread::Thread(QObject *parent) : QThread(parent)
{ }

void Thread::run(std::vector<std::string>& tokenized, NgramModel& m, const size_t& n_grams,
                 const QList<QString>& files) {

    emit start();
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    size_t suggestions;
    suggestions = 3;
    m = NgramModel{n_grams, suggestions};
    for (auto &file : files) {
        std::string text_data = read_binary_file(file.toUtf8().constData());
        tokenized = tokenize_text(text_data);
        m.update(tokenized);
        std::cout << "Ok!" << std::endl;
    }
    emit progressChanged(100);

    std::cout << "Here!" << std::endl;
}
