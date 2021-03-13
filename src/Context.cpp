//
// Created by nataliia on 13.03.21.
//

#include "../inc/Context.h"

Context::Context(Ngram &ngram) {
    context = std::move(ngram.context);
}

Context::Context(std::vector<std::string> &context) :
        context(std::move(context)){
}

std::size_t Context::hash_value() {
    size_t seed = 0;
    for (const auto &word: context) {
        boost::hash_combine(seed, word);
    }
    return seed;
}

bool Context::operator==(const Context &other) const {
    return context == other.context;
}

