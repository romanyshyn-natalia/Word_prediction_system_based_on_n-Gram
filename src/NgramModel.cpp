// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by nataliia on 26.02.21.
//


#include "../inc/NgramModel.h"


std::vector<Ngram>
NgramModel::get_ngrams(std::vector<std::string> &tokens) const {
    tokens.insert(tokens.begin(), n - 1, "<s>");
    std::vector<Ngram> ngram_list;
    Ngram current_ngram;
    for (size_t i = n - 1; i < tokens.size(); ++i) {
        std::vector<std::string> history;
        for (int p = n - 2; p >= 0; --p) {
            history.push_back(tokens[i - p - 1]);
        }
        current_ngram.context = history;
        current_ngram.token = tokens[i];
        ngram_list.push_back(current_ngram);
    }
    return ngram_list;

}

void NgramModel::update(std::vector<std::string> &tokens) {
    auto ngrams = get_ngrams(tokens);
    for (auto &ngram: ngrams) {
        std::vector<std::string> new_context{ngram.context};
        ngram_count[ngram] += 1;
        context[new_context].push_back(ngram.token);
    }
}


double NgramModel::probability(const std::vector<std::string> &current_context, std::string &token) {
    Ngram new_ngram(current_context, token);

    auto count_of_token = double(ngram_count[new_ngram]);
    auto count_of_ngram = double(context[current_context].size());
    if (count_of_ngram == 0.0) return 0.0;

    return count_of_token / count_of_ngram;
}

std::string NgramModel::random_token(std::vector<std::string> &current_context) {
    double r = random_double();
    double sum_ = 0.0;
    std::unordered_map<std::string, double> probability_map;
    for (auto &curr_token: context[current_context]) {
        probability_map[curr_token] = probability(current_context, curr_token);
    }
    for (auto &curr_token: probability_map) {
        sum_ += curr_token.second;
        if (sum_ > r) {
            return curr_token.first;
        }
    }
}

std::string NgramModel::generate_text(size_t token_count) {
    std::vector<std::string> curr_context;
    for (size_t i = 0; i < n - 1; ++i) {
        curr_context.emplace_back("<s>");
    }
    std::vector<std::string> result;
    for (size_t i = 0; i < token_count; ++i) {
        std::vector<std::string> context_queue{curr_context};
        std::string token = random_token(context_queue);
        result.push_back(token);
        curr_context.erase(curr_context.begin());
        curr_context.push_back(token);
    }
    std::string s;
    for (const auto &piece : result) {
        s += piece;
        s += " ";
    }
    return s;
}


double random_double() {
    srand(565000);
    double lower_bound = 0;
    double upper_bound = 1;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;
    double a_random_double = unif(re);
    return a_random_double;
}

