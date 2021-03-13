//
// Created by nataliia on 13.03.21.
//

#ifndef N_GRAM_HASHER_H
#define N_GRAM_HASHER_H

#include "NgramModel.h"

struct hasher {
    std::size_t operator()(const Context &context) const {
        size_t seed = 0;
        for (const auto &word: context.context) {
            boost::hash_combine(seed, word);
        }
        return seed;
    }
};


#endif //N_GRAM_HASHER_H
