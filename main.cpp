// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ngram_model.h"
#include "word_tokenizer.h"
#include "time_measuring.h"
#include <sys/resource.h>


int main() {

#define PRINT_INTERACTION
#ifdef PRINT_INTERACTION
    // connect backend
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("test");
    db.setPassword("postgres");
    db.setUserName("postgres");

    std::vector<std::string> tokenized;
    std::cout << "Reading the file..." << std::endl;
    std::string text_data = read_binary_file("../resources/51.txt");
    std::cout << "Tokenizing..." << std::endl;
    tokenized = tokenize_text(text_data);

    std::string str_n_grams, str_suggestions, str_k;

    if (db.open()) {
        qDebug() << "Open!!!";
        QString table_context = "";
        for (size_t i = 1; i < 3; i++) {
            table_context += ", word_" + QString::number(i) + " Decimal(20,0) NOT NULL";
        }
        QSqlQuery query("DROP TABLE IF EXISTS word;"
                        "DROP TABLE IF EXISTS context;"
                        "DROP TABLE IF EXISTS grams;"
                        "CREATE TABLE word ("
                        "  word_id Decimal(20,0) primary key,"
                        "  word_v TEXT"
                        ");"
                        "CREATE TABLE context ("
                        "  context_id Decimal(20,0) NOT NULL" + table_context +
                        ");"
                        "CREATE TABLE grams ("
                        "  context_id Decimal(20,0) NOT NULL,"
                        "  token_id Decimal(20,0) NOT NULL"
                        ");");
        std::cout << "END" << std::endl;
    }

    struct rusage r_usage;
    ngram_model<unsigned long> m{db, 3, 3, 1};
    auto before = get_current_time_fenced();
    m.update(tokenized);
    auto time_to_calculate_reading = get_current_time_fenced() - before;
    std::cout << "model updating time: " << to_us(time_to_calculate_reading) << std::endl;
    int ret = getrusage(RUSAGE_SELF, &r_usage);
    std::cout << r_usage.ru_maxrss << std::endl;

#endif //PRINT_INTERACTION

    return 0;
}