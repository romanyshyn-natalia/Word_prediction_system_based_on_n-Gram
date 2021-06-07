#ifndef N_GRAM_NGRAM_MODEL_H
#define N_GRAM_NGRAM_MODEL_H


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
    explicit ngram_model(size_t n_grams = 2, size_t suggestions = 1, double k_smoothing = 1) : number_of_grams{n_grams},
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
        for (const auto &word : new_hashed) {
            hashed_tokens[word.first] = word.second;
        }
        std::vector<unsigned long> hash_tokens = hashed_text(new_tokens);
#pragma omp parallel for shared(hash_tokens) default (none)
        for (size_t i = number_of_grams - 1; i < hash_tokens.size(); ++i) {
#pragma omp critical
            {
                tokens_count[hash_tokens[i]]++;
            }
            std::vector<unsigned long> history;
            for (int p = static_cast<int>(number_of_grams) - 2; p >= 0; --p) {
                history.push_back(hash_tokens[i - p - 1]);
            }
            ngram<unsigned long> ngram(history, hash_tokens[i]);
            std::vector<unsigned long> new_context{ngram.getContext()};
#pragma omp critical
            {
                ngram_count[ngram]++;
                context[new_context].push_back(ngram.getToken());
            }
        }
    };

    /*
     * Calculate probabilities of seeing given token in given context.
     * @param current_context vector of context.
     * @param token.
     * @return double.
     */
    double probability(const std::vector<T> &current_context, const T &token) {
        ngram new_ngram(current_context, token);

        auto count_of_token = static_cast<double>(ngram_count[new_ngram]);
        auto count_of_ngram = context[current_context].size();

        if (count_of_ngram == 0) return 0.0;
        // smoothing
        return (count_of_token + k) /
               (static_cast<double>(count_of_ngram) + k * static_cast<double>(hashed_tokens.size())); //-V113
    };

    /*
     * Calculates most probable tokens.
     * @param current_context vector of string.
     * @param vector of context token.
     * @return vector of words and its probability.
     */
    std::vector<std::pair<T, double>> probable_tokens(const std::vector<T> &current_context) {
        std::unordered_map<T, double> token_probability_map;
        for (auto &curr_token: context[current_context]) {
            ngram current_ngram(current_context, curr_token);
            if (probability_map.find(current_ngram) == probability_map.end()) {
                probability_map[current_ngram] = probability(current_context, curr_token);
            }
            token_probability_map[curr_token] = probability_map[current_ngram];
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
        auto tokens = probable_tokens(current_context);

        result_t.reserve(tokens.size());
        for (const auto &t: tokens) {
            if (std::find(result_t.begin(), result_t.end(), t.first) == result_t.end())
                result_t.emplace_back(t.first);
        }
        std::vector<std::string> result_words;
        for (const auto &t: result_t) {
            result_words.emplace_back(hashed_tokens[t]);
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

    std::unordered_map<std::vector<T>, std::vector<T>, vector_hasher<T>> context;

    std::unordered_map<ngram<T>, unsigned long long, ngram_hasher<T>> ngram_count;


private:
    size_t number_of_grams;
    size_t number_of_suggestions;
    std::unordered_map<ngram<T>, double, ngram_hasher<T>> probability_map;
    std::unordered_map<T, double> tokens_count;
    std::unordered_map<unsigned long, std::string> hashed_tokens;
    double k;
};


#endif //N_GRAM_NGRAM_MODEL_H