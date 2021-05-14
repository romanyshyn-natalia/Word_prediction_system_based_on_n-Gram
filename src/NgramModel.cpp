// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <iostream>
#include <fstream>
#include "../inc/NgramModel.h"


void NgramModel::update(const std::vector<std::string> &tokens) {
    std::vector<std::string> new_tokens = tokens;
    new_tokens.insert(new_tokens.begin(), number_of_grams - 1, "<s>");

    #pragma omp parallel for shared(new_tokens) default (none)
    for (size_t i = number_of_grams - 1; i < new_tokens.size(); ++i) {
        #pragma omp critical
        {
            tokens_count[new_tokens[i]]++;
        }
        std::vector<std::string> history;
        for (int p = number_of_grams - 2; p >= 0; --p) {
            history.push_back(new_tokens[i - p - 1]);
        }
        Ngram ngram(history, new_tokens[i]);
        std::vector<std::string> new_context{ngram.getContext()};
        #pragma omp critical
        {
            ngram_count[ngram]++;
            context[new_context].push_back(ngram.getToken());
        }
    }
}

double NgramModel::probability(const std::vector<std::string> &current_context, const std::string &token) {
    Ngram new_ngram(current_context, token);

    auto count_of_token = static_cast<double>(ngram_count[new_ngram]);
    auto count_of_ngram = context[current_context].size();

    if (count_of_ngram == 0) return 0.0;
    return (count_of_token + k) / (count_of_ngram + k * tokens_list.size()); //-V113
}

std::vector<std::pair<std::string, double>>
NgramModel::probable_tokens(const std::vector<std::string> &current_context) {
    std::unordered_map<std::string, double> token_probability_map;
    for (auto &curr_token: context[current_context]) {
        Ngram current_ngram(current_context, curr_token);
        if (probability_map.find(current_ngram) == probability_map.end()) {
            probability_map[current_ngram] = probability(current_context, curr_token);
        }
        token_probability_map[curr_token] = probability_map[current_ngram];
    }
    size_t curr_num_suggest = (token_probability_map.size() > number_of_suggestions) ?
                              number_of_suggestions : token_probability_map.size();
    std::vector<std::pair<std::string, double>> most_probable(curr_num_suggest);
    std::partial_sort_copy(token_probability_map.begin(), token_probability_map.end(),
                           most_probable.begin(), most_probable.end(),
                           [](auto &left, auto &right) {
                               return left.second > right.second;
                           });
    std::vector<std::pair<std::string, double>> additional_most_probable(number_of_suggestions - curr_num_suggest);
    if (curr_num_suggest < number_of_suggestions) {
        std::partial_sort_copy(tokens_count.begin(), tokens_count.end(),
                               additional_most_probable.begin(), additional_most_probable.end(),
                               [](auto &left, auto &right) {
                                   return left.second > right.second;
                               });
        most_probable.reserve(
                most_probable.size() + std::distance(additional_most_probable.begin(), additional_most_probable.end()));
        most_probable.insert(most_probable.end(), additional_most_probable.begin(), additional_most_probable.end());
    }
    return most_probable;
}

std::vector<std::string> NgramModel::autocomplete(const std::vector<std::string> &user_text_tokenized) {
    std::vector<std::string> current_context;
    if (user_text_tokenized.size() <= (number_of_grams - 1)) {
        current_context.assign(user_text_tokenized.begin(), user_text_tokenized.end());
    } else {
        current_context.assign(user_text_tokenized.end() - number_of_grams + 1, user_text_tokenized.end());
    }
    for (size_t i = 0; i < number_of_grams - 1 - current_context.size(); ++i) {
        current_context.emplace_back("<s>");
        std::rotate(current_context.rbegin(), current_context.rbegin() + 1, current_context.rend());
    }
    std::vector<std::string> result_words;
    auto tokens = probable_tokens(current_context);

    result_words.reserve(tokens.size());
    for (const auto &word: tokens) {
        result_words.emplace_back(word.first);
    }
    return result_words;
}




