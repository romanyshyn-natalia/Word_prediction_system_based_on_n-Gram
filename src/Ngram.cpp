// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../inc/Ngram.h"

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
    std::vector<std::string> new_context;
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
        context.push_back(tokens[i]); // I perfectly sure -- there is some bug here! new_context should be used?
    }
    token = tokens[tokens.size() - 1];
    context =  std::move(new_context); // Here move is appropriate, though looks like code can be refactored more cleanly
}

Ngram::Ngram(const std::vector<std::string> &context, std::string &token) :
        context(context),
        token(token) {}

