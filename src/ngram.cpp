// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <fstream>
#include "ngram.h"


bool ngram::operator==(const ngram &right) const {
    for (size_t i = 0; i < context.size(); ++i) {
        if (this->context[i] != right.context[i]) {
            return false;
        }
    }
    return this->token == right.token;
}

ngram::ngram(std::vector<std::string> &context, std::string &token) :
        context(context),
        token(token) {}

ngram::ngram(const std::vector<std::string> &tokens) {
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        context.push_back(tokens[i]);
    }
    token = tokens[tokens.size() - 1];
}

ngram::ngram(const std::vector<std::string> &context, const std::string &token) :
        context(context),
        token(token) {}

std::ofstream &ngram::operator<<(std::ofstream &stream) {
    for (const auto &word: context) {
        stream << word << " ";
    }
    stream << token << " ";
    return stream;
}
