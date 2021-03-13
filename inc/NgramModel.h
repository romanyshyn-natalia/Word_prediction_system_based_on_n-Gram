//
// Created by nataliia on 26.02.21.
//

#ifndef N_GRAM_NGRAMMODEL_H
#define N_GRAM_NGRAMMODEL_H


#include <map>
#include "boost/array.hpp"
#include <boost/algorithm/string/join.hpp>
#include "Ngram.h"
#include <unordered_map>
#include <iostream>
#include "Context.h"
#include "hasher.h"

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

    double probability(const std::vector<std::string> &current_context, const std::string &token);

    std::string random_token(const std::vector<std::string> &current_context);

    std::string generate_text(size_t token_count);

    static void print_ngrams(const std::vector<Ngram> &obtained_ngrams);

    std::unordered_map<Context, std::vector<std::string>, hasher> context;

    std::unordered_map<Context, unsigned long long, hasher> ngram_count;

private:
    size_t n;
};


#endif //N_GRAM_NGRAMMODEL_H
