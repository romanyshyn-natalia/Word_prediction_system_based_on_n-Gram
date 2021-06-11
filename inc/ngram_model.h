#ifndef N_GRAM_NGRAM_MODEL_H
#define N_GRAM_NGRAM_MODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlQueryModel>
#include "QSqlError"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include "ngram_hasher.h"
#include "vector_hasher.h"
#include "word_tokenizer.h"


template<typename T>
class ngram_model {
public:
    explicit ngram_model(QSqlDatabase &db_, size_t n_grams = 2, size_t suggestions = 1, double k_smoothing = 1) : db(
            db_), number_of_grams{n_grams},
                                                                                                                  number_of_suggestions{
                                                                                                                          suggestions},
                                                                                                                  k{k_smoothing} {}

    ~ngram_model() = default;

    /*
     * Updates model with extracted ngrams from tokens
     * @param tokens vector of strings with tokenized words.
     * @return nothing.
     */
    void update(const std::vector<std::string> &tokens) {
        std::vector<std::string> new_tokens = tokens;
        new_tokens.insert(new_tokens.begin(), number_of_grams - 1, "<s>");
        auto new_hashed = tokens_hasher(new_tokens);

        QString query_str = "";
        auto *query_word = new QSqlQuery(db);
        for (const auto &word : new_hashed) {
            query_str += "(" + QString::number(word.first) + ",";
            QString wor = QString::fromStdString(word.second);
            wor.remove("'");
            query_str += "'" + wor + "'),";
        }
        if (!query_str.isEmpty()) {
            query_str.chop(1);
        }
        query_word->prepare("\n"
                            "INSERT INTO word (word_id, word_v)\n"
                            "VALUES" + query_str +
                            "ON CONFLICT (word_id) DO NOTHING;");
        if (!query_word->exec()) {
            qDebug() << query_word->lastError().text();
        }
        std::vector<unsigned long> hash_tokens = hashed_text(new_tokens);

// #pragma omp parallel for shared(hash_tokens) default (none)
        for (size_t i = number_of_grams - 1; i < hash_tokens.size(); ++i) {
// #pragma omp critical
            {
                tokens_count[hash_tokens[i]]++;
            }
            std::vector<unsigned long> history;
            auto *query_context = new QSqlQuery(db);
            int num = static_cast<int>(number_of_grams) - 1;

            QString query_str_context = "";
            for (int p = num - 1; p >= 0; --p) {
                query_str_context += " word_" +  QString::number(num - p) + " = " + QString::number(hash_tokens[i - p - 1]) + " AND";
            }
            if (!query_str_context.isEmpty()) {
                query_str_context.chop(4);
            }
            query_context->prepare("SELECT context_id FROM context WHERE" + query_str_context + ";");
            if (!query_context->exec()) {
                qDebug() << query_context->lastError().text();
            }

            unsigned long cont_id = 0;
            if (query_context->next()) {
                cont_id = (unsigned long) query_context->value(0).toULongLong();
            }

            if (cont_id != 0) {
                for (int p = num - 1; p >= 0; --p) {
                    history.push_back(hash_tokens[i - p - 1]);
                }
                ngram<unsigned long> ngram(history, hash_tokens[i]);
                auto *query_grams = new QSqlQuery(db);
                query_grams->prepare(
                        "INSERT INTO grams (context_id, token_id) VALUES (" + QString::number(cont_id) + "," +
                        QString::number(hash_tokens[i]) + ");");
                if (!query_grams->exec()) {
                    qDebug() << query_grams->lastError().text();
                }

            } else {
                QString query_str_word = "INSERT INTO context (context_id";
                for (size_t order = 1; order < number_of_grams; order++) {
                    query_str_word += ", word_" + QString::number(order);
                }
                query_str_word += ") VALUES (" + QString::number(context_count);
                for (int p = num - 1; p >= 0; --p) {
                    history.push_back(hash_tokens[i - p - 1]);
                    query_str_word += "," + QString::number(hash_tokens[i - p - 1]);
                }
                query_context->prepare(query_str_word + ");");
                if (!query_context->exec()) {
                    qDebug() << query_context->lastError().text();
                }

                auto *query_grams = new QSqlQuery(db);
                query_grams->prepare(
                        "INSERT INTO grams (context_id, token_id) VALUES (" + QString::number(context_count) + "," +
                        QString::number(hash_tokens[i]) + ");");
                if (!query_grams->exec()) {
                    qDebug() << query_grams->lastError().text();
                }
                context_count++;
            }
        }
    };

    /*
     * Calculate probabilities of seeing given token in given context.
     * @param current_context vector of context.
     * @param token.
     * @return double.
     */
    double probability(const unsigned long &cont_id, const T &token) {
        auto *query = new QSqlQuery(db);
        query->prepare("SELECT COUNT(*) FROM grams WHERE context_id = " + QString::number(cont_id) +
                       " AND token_id = " + QString::number(token) + ";");
        if (!query->exec()) {
            qDebug() << query->lastError().text();
        }

        auto count_of_token = static_cast<double>(query->size());

        query->prepare("SELECT COUNT(*) FROM grams WHERE context_id = " + QString::number(cont_id) + ";");
        if (!query->exec()) {
            qDebug() << query->lastError().text();
        }
        auto count_of_ngram = static_cast<double>(query->size());

        if (count_of_ngram == 0) return 0.0;
        query->prepare("SELECT COUNT(*) FROM word");
        query->exec();
        // smoothing
        return (count_of_token + k) /
               (static_cast<double>(count_of_ngram) + k * query->size()); //-V113
    };

    /*
     * Calculates most probable tokens.
     * @param current_context vector of string.
     * @param vector of context token.
     * @return vector of words and its probability.
     */
    std::vector<std::pair<T, double>> probable_tokens(const std::vector<T> &current_context, const unsigned long &cont_id) {
        std::unordered_map<T, double> token_probability_map;
        if (cont_id != 0) {
            auto *query = new QSqlQuery(db);
            query->prepare(
                    "SELECT token_id FROM grams WHERE context_id = " + QString::number(cont_id) + ";");
            if (!query->exec()) {
                qDebug() << query->lastError().text();
            }

            while (query->next()) {
                ngram current_ngram(current_context, (unsigned long) query->value(0).toULongLong());
                if (probability_map.find(current_ngram) == probability_map.end()) {
                    probability_map[current_ngram] = probability(cont_id,
                                                                 (unsigned long) query->value(0).toULongLong());
                }
                token_probability_map[(unsigned long) query->value(0).toULongLong()] = probability_map[current_ngram];
            }
        }
        size_t curr_num_suggest = (token_probability_map.size() > number_of_suggestions) ?
                                  number_of_suggestions : token_probability_map.size();
        std::vector<std::pair<T, double>> most_probable(curr_num_suggest);
        std::partial_sort_copy(token_probability_map.begin(), token_probability_map.end(),
                               most_probable.begin(), most_probable.end(),
                               [](auto &left, auto &right) {
                                   return left.second > right.second;
                               });
        std::vector<std::pair<T, double>> additional_most_probable(number_of_suggestions - curr_num_suggest);
        if (curr_num_suggest < number_of_suggestions) {
            std::partial_sort_copy(tokens_count.begin(), tokens_count.end(),
                                   additional_most_probable.begin(), additional_most_probable.end(),
                                   [](auto &left, auto &right) {
                                       return left.second > right.second;
                                   });
            most_probable.reserve(
                    most_probable.size() +
                    std::distance(additional_most_probable.begin(), additional_most_probable.end()));
            most_probable.insert(most_probable.end(), additional_most_probable.begin(), additional_most_probable.end());
        }
        return most_probable;
    };

    /*
     * Generate most probable end of phrase for user input.
     * @param user_text_tokenized vector of tokens.
     * @return vector of strings.
     */
    std::vector<std::string> autocomplete(const std::vector<T> &user_text_tokenized) {
        std::vector<T> current_context;
        if (user_text_tokenized.size() <= (number_of_grams - 1)) {
            current_context.assign(user_text_tokenized.begin(), user_text_tokenized.end());
        } else {
            current_context.assign(user_text_tokenized.end() - number_of_grams + 1, user_text_tokenized.end());
        }
        for (size_t i = 0; i < number_of_grams - 1 - current_context.size(); ++i) {
            current_context.emplace_back(std::hash<std::string>{}("<s>"));
            std::rotate(current_context.rbegin(), current_context.rbegin() + 1, current_context.rend());
        }
        std::vector<T> result_t;


        QString query_str = "";
        for (size_t i = 1; i <= current_context.size(); ++i) {
            query_str += " word_" +  QString::number(i) + " = " + QString::number(current_context[i]) + " AND";
        }
        if (!query_str.isEmpty()) {
            query_str.chop(4);
        }

        auto *query_context_id = new QSqlQuery(db);
        query_context_id->prepare("SELECT context_id FROM context WHERE" + query_str + ";");
        if (!query_context_id->exec()) {
            qDebug() << query_context_id->lastError().text();
        }

        unsigned long cont_id = 0;
        if (query_context_id->next()) {
            cont_id = (unsigned long) query_context_id->value(0).toULongLong();
        }

        auto tokens = probable_tokens(current_context, cont_id);

        result_t.reserve(tokens.size());
        for (const auto &t: tokens) {
            if (std::find(result_t.begin(), result_t.end(), t.first) == result_t.end())
                result_t.emplace_back(t.first);
        }
        std::vector<std::string> result_words;

        for (const auto &t: result_t) {
            query_context_id->prepare("SELECT word_v FROM word WHERE word_id = " + QString::number(t) + ";");
            if (!query_context_id->exec()) {
                qDebug() << query_context_id->lastError().text();
            }
            if (query_context_id->next()) {
                result_words.emplace_back(query_context_id->value(0).toString().toStdString());
            }
        }
        return result_words;
    };

    /*
    * Checks whether the model is empty.
    * @param nothing.
    * @return bool.
    */
    bool is_empty() {
        return tokens_count.empty();
    }

//    std::unordered_map<std::vector<T>, std::vector<T>, vector_hasher<T>> context;
//
//    std::unordered_map<ngram<T>, unsigned long long, ngram_hasher<T>> ngram_count;


private:
    QSqlDatabase &db;
    size_t number_of_grams;
    size_t number_of_suggestions;
    size_t context_count = 1;
    std::unordered_map<ngram<T>, double, ngram_hasher<T>> probability_map;
    std::unordered_map<T, double> tokens_count;
//    std::unordered_map<unsigned long, std::string> hashed_tokens;
    double k;
};


#endif //N_GRAM_NGRAM_MODEL_H