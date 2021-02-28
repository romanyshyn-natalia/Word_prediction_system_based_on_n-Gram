//
// Created by nataliia on 26.02.21.
//

#ifndef N_GRAM_NGRAMMODEL_H
#define N_GRAM_NGRAMMODEL_H

#include <cstdlib>
#include <map>
#include <vector>
#include <string>
#include "boost/array.hpp"


class NgramModel {
public:
    //constructors
    NgramModel() : n{1} {
        context = {};
        ngram_count = {};
    };

    explicit NgramModel(size_t n) : n{n} {
        context = {};
        ngram_count = {};

    }

    //destructor
    ~NgramModel() = default;

    //methods
    std::vector<std::tuple<std::vector<std::string>, std::string>> get_ngrams(std::vector<std::string>
                                                                              &tokens) const;

    void update(const std::string &sentence);

    double probability(const std::vector<std::string> &current_context, const std::string &token);

    std::string random_token(const std::vector<std::string> &current_context);

    std::string generate_text(size_t token_count);

    static void print_ngrams(const std::vector<std::tuple<std::vector<std::string>, std::string>> &obrained_ngrams);

private:
    size_t n;
    std::map<std::vector<std::string>, std::vector<std::string>> context;
    std::map<std::vector<std::string>, unsigned long long> ngram_count;
};


#endif //N_GRAM_NGRAMMODEL_H
