#ifndef N_GRAM_NGRAM_HASHER_H
#define N_GRAM_NGRAM_HASHER_H

#include "boost/container_hash/hash.hpp"
#include "model/ngram.h"

template <typename T>
struct ngram_hasher {
    std::size_t operator()(const ngram<T> &ngram) const {
        size_t seed = 0;
        for (const auto &word: ngram.getContext()) {
            boost::hash_combine(seed, word);
        }
        boost::hash_combine(seed, ngram.getToken());
        return seed;
    }
};

#endif //N_GRAM_NGRAM_HASHER_H