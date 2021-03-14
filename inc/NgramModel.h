//
// Created by nataliia on 26.02.21.
//

#ifndef N_GRAM_NGRAMMODEL_H
#define N_GRAM_NGRAMMODEL_H


#include <boost/algorithm/string/join.hpp>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <random>
#include "boost/array.hpp"
#include "Ngram.h"
#include "Context.h"
#include "context_hasher.h"
#include "ngram_hasher.h"


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
    std::vector<Ngram> get_ngrams(std::vector<std::string> &tokens) const;

    void update(std::vector<std::string> &tokens);

    double probability(Context &current_context, std::string &token);

    std::string random_token(Context &current_context);

    std::string generate_text(size_t token_count);

    std::unordered_map<Context, std::vector<std::string>, context_hasher> context;

    std::unordered_map<Ngram, unsigned long long, ngram_hasher> ngram_count;

private:
    size_t n;
};


double random_double();

#endif //N_GRAM_NGRAMMODEL_H
