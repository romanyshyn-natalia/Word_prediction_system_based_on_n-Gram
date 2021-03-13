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
        Context new_context(ngram);
        ngram_count[new_context] += 1;
        context[new_context].push_back(ngram.token);
    }
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

void NgramModel::print_ngrams(const std::vector<Ngram> &obtained_ngrams) {
    for (const auto &elem: obtained_ngrams) {
        std::cout << "(" << boost::algorithm::join(elem.context, " ");
        std::cout << ": " << elem.token << "),    " << std::endl;
    }
}


