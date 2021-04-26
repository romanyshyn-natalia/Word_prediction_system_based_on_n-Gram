#ifndef NGRAM_HASHER_H
#define NGRAM_HASHER_H

#include "ngram.h"
#include "boost/container_hash/hash.hpp"

struct ngram_hasher {
    std::size_t operator()(const Ngram &ngram) const {
        size_t seed = 0;
        for (const auto &word: ngram.getContext()) {
            boost::hash_combine(seed, word);
        }
        boost::hash_combine(seed, ngram.getToken());
        return seed;
    }
};

#endif // NGRAM_HASHER_H
