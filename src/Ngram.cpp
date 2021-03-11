//
// Created by nataliia on 09.03.21.
//

#include "../inc/Ngram.h"

bool Ngram::operator==(const Ngram &right) {
    for (size_t i = 0; i < context.size(); ++i) {
        if (this->context[i] != right.context[i]) {
            return false;
        }
    }
    return this->token == right.token;
}

std::size_t Ngram::hash_value(const Ngram &ngram) {
    size_t seed = 0;
    for (const auto &word: context) {
        boost::hash_combine(seed, word);
    }
    boost::hash_combine(seed, token);
    return seed;
}

Ngram::Ngram(std::vector<std::string> &context, std::string &token) :
        context(std::move(context)),
        token(std::move(token)) {}

Ngram::Ngram(std::vector<std::string> &tokens) {
    std::vector<std::string> new_context;
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        context.push_back(tokens[i]);
    }
    token = tokens[tokens.size() - 1];
    context = new_context;
}

Ngram::Ngram(Ngram &ngram) :
        context(std::move(ngram.context)),
        token(std::move(ngram.token)) {
}



