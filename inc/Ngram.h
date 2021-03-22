//
// Created by nataliia on 09.03.21.
//

#ifndef N_GRAM_NGRAM_H
#define N_GRAM_NGRAM_H

#include <vector>
#include <string>
#include <cstdlib>
#include "boost/container_hash/hash.hpp"


class Ngram {

public:
    Ngram() = default;

    Ngram(std::vector<std::string> &context, std::string &token);

    Ngram(const std::vector<std::string> &context, std::string &token);

    Ngram(std::vector<std::string> &tokens);

    Ngram(const std::vector<std::string> &tokens);

    Ngram(Ngram &ngram) = default;

    Ngram(const Ngram &ngram) = default;

    ~Ngram() = default;

    bool operator==(const Ngram &right) const;

    Ngram &operator=(Ngram &other) = default;

    Ngram &operator=(const Ngram &other) = default;

    std::vector<std::string> context;
    std::string token;
};


#endif //N_GRAM_NGRAM_H
