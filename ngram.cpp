#include "ngram.h"

#include <utility>

bool Ngram::operator==(const Ngram &right) const {
    for (size_t i = 0; i < context.size(); ++i) {
        if (this->context[i] != right.context[i]) {
            return false;
        }
    }
    return this->token == right.token;
}

Ngram::Ngram(std::vector<std::string> &context, std::string &token) :
        context(context),
        token(token) {}

Ngram::Ngram(const std::vector<std::string> &tokens) {
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        context.push_back(tokens[i]);
    }
    token = tokens[tokens.size() - 1];
}

Ngram::Ngram(const std::vector<std::string> &context, const std::string &token) :
        context(context),
        token(token) {}
