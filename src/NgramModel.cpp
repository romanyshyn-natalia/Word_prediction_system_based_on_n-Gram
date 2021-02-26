//
// Created by nataliia on 26.02.21.
//

#include <iostream>
#include "../inc/NgramModel.h"


std::vector<std::tuple<std::vector<std::string>, std::string>>
NgramModel::get_ngrams(std::vector<std::string> &tokens) const {
    tokens.insert(tokens.begin(), n - 1, "<s>");
    std::vector<std::tuple<std::vector<std::string>, std::string>> ngram_list;
    std::tuple<std::vector<std::string>, std::string> current_ngram;
    for (size_t i = n - 1; i < tokens.size(); ++i) {
        std::vector<std::string> history;
        for (int p = n - 2; p >= 0; --p) {
            history.push_back(tokens[i - p - 1]);
        }
        std::get<0>(current_ngram) = history;
        std::get<1>(current_ngram) = tokens[i];
        ngram_list.push_back(current_ngram);
    }
    return ngram_list;

}

void NgramModel::update(const std::string &sentence) {


}

double NgramModel::probability(const std::vector<std::string> &current_context, const std::string &token) {
    return 0;
}

std::string NgramModel::random_token(const std::vector<std::string> &current_context) {
    return std::string();
}

std::string NgramModel::generate_text(size_t token_count) {
    return std::string();
}

void NgramModel::print_ngrams(const std::vector<std::tuple<std::vector<std::string>, std::string>> &obtained_ngrams) {
    for (const auto &elem: obtained_ngrams) {
        std::cout << "((";
        for (const auto &gram: std::get<0>(elem)) {
            std::cout << gram << " ";
        }
        std::cout << "), " << std::get<1>(elem) << "),    " << std::endl;
    }
}


