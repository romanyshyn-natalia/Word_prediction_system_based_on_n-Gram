//
// Created by nataliia on 13.03.21.
//

#include "../inc/Context.h"

Context::Context(Ngram &ngram) {
    context = (ngram.context);
}

Context::Context(std::vector<std::string> &context) :
        context(context){
}

bool Context::operator==(const Context &other) const {
    return context == other.context;
}

