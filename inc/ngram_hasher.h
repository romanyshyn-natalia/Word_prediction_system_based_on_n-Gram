#ifndef N_GRAM_NGRAM_HASHER_H
#define N_GRAM_NGRAM_HASHER_H

#include "Ngram.h"
#include "boost/container_hash/hash.hpp"

struct ngram_hasher {
    std::size_t operator()(const Ngram &ngram) const {
        size_t seed = 0;
        for (const auto &word: ngram.context) {
            boost::hash_combine(seed, word);
        }
        boost::hash_combine(seed, ngram.token);
        return seed;
    }
};

#endif //N_GRAM_NGRAM_HASHER_H
