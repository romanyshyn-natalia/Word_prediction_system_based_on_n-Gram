//
// Created by nataliia on 13.03.21.
//

#ifndef N_GRAM_CONTEXT_H
#define N_GRAM_CONTEXT_H

#include "./Ngram.h"

class Context {

public:
    std::vector<std::string> context;

    Context() = default;

    Context(Ngram &ngram);

    Context(const Context &context) = default;

    Context(std::vector<std::string> &context);

    ~Context() = default;

    bool operator==(const Context &other) const;

    std::size_t hash_value();

};


#endif //N_GRAM_CONTEXT_H
