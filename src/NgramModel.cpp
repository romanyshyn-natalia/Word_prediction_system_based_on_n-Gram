// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../inc/NgramModel.h"


std::vector<Ngram>
NgramModel::get_ngrams(const std::vector<std::string> &tokens) const {
    std::vector<std::string> new_tokens = tokens;
    new_tokens.insert(new_tokens.begin(), number_of_grams - 1, "<s>");
    std::vector<Ngram> ngram_list;
    for (size_t i = number_of_grams - 1; i < new_tokens.size(); ++i) {
        std::vector<std::string> history;
        for (int p = number_of_grams - 2; p >= 0; --p) {
            history.push_back(new_tokens[i - p - 1]);
        }
        ngram_list.emplace_back(history, new_tokens[i]);
    }
    return ngram_list;
}

void NgramModel::update(const std::vector<std::string> &tokens) {
    auto ngrams = get_ngrams(tokens);
    for (auto &ngram: ngrams) {
        std::vector<std::string> new_context{ngram.getContext()};
        ngram_count[ngram] += 1;
        context[new_context].push_back(ngram.getToken());
    }
}


double NgramModel::probability(const std::vector<std::string> &current_context, const std::string &token) {
    Ngram new_ngram(current_context, token);

    auto count_of_token = static_cast<double>(ngram_count[new_ngram]);
    auto count_of_ngram = context[current_context].size();
    if (count_of_ngram == 0) return 0.0;

    return count_of_token / count_of_ngram; //-V113
}

std::string NgramModel::random_token(const std::vector<std::string> &current_context) {
    double r = 0.5;
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
    return "";
}

std::string NgramModel::generate_text(const size_t &token_count) {
    std::vector<std::string> curr_context; //! Replace by std::queue! You need to pop_front() frequently -- simulated
    //! by curr_context.erase(curr_context.begin()) with O(N) complexty.
    for (size_t i = 0; i < number_of_grams - 1; ++i) {
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




