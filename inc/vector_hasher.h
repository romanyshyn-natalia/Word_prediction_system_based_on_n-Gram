#ifndef VECTOR_HASHER_H
#define VECTOR_HASHER_H

#include <vector>
#include <string>
#include "boost/container_hash/hash.hpp"


struct vector_hasher {
    std::size_t operator()(const std::vector<std::string> &input) const {
        size_t seed = 0;
        for (const auto &word: input) {
            boost::hash_combine(seed, word);
        }
        return seed;
    }
};

#endif // VECTOR_HASHER_H