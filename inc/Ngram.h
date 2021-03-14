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

    //Ngram(Context &context, std::string &token);

    Ngram(std::vector<std::string> &tokens);

    Ngram(Ngram &ngram);

    Ngram(const Ngram &ngram) = default;

    ~Ngram() = default;

    std::vector<std::string> context;
    std::string token;

    bool operator==(const Ngram &right) const;

    Ngram &operator=(Ngram &other) = default;

    Ngram &operator=(const Ngram &other) = default;


};


#endif //N_GRAM_NGRAM_H
