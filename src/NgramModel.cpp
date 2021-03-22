// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//
// Created by nataliia on 26.02.21.
//


#include "../inc/NgramModel.h"


std::vector<Ngram>
NgramModel::get_ngrams(std::vector<std::string> &tokens) const {
    tokens.insert(tokens.begin(), n - 1, "<s>"); //! O.F.: This do needs refactoring!
    std::vector<Ngram> ngram_list;
    for (size_t i = n - 1; i < tokens.size(); ++i) {
        std::vector<std::string> history;
        for (int p = n - 2; p >= 0; --p) {
            history.push_back(tokens[i - p - 1]);
        }
        ngram_list.emplace_back(history, tokens[i]);
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

    auto count_of_token = static_cast<double>(ngram_count[new_ngram]);
    auto count_of_ngram = context[current_context].size(); // Друге приведення було надмірним і створювало зайві попередження.
    if (count_of_ngram == 0) return 0.0; 

    return count_of_token / count_of_ngram; //-V113
}

std::string NgramModel::random_token(const std::vector<std::string> &current_context) {
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
#if defined __GNUC__ || defined __clang__
#warning What should be returned here? It was UB. O.F.
#endif
    return std::string{}; //!
}

std::string NgramModel::generate_text(size_t token_count) {
    std::vector<std::string> curr_context; //! Replace by std::queue! You need to pop_front() frequently -- simulated
                                            //! by curr_context.erase(curr_context.begin()) with O(N) complexty.
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


//! This function is plane wrong: Put creation of the unif and re to main() or to some singleton class
//! -- they should be unique through the program.
//! Additionally, srand() has no relation to C++ random facilities.
double random_double() {
    srand(565000);
    double lower_bound = 0;
    double upper_bound = 1;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;
    double a_random_double = unif(re);
    return a_random_double;
}

