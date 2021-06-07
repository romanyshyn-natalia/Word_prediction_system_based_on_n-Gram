#ifndef N_GRAM_NGRAM_H
#define N_GRAM_NGRAM_H

#include <vector>
#include <string>

template<typename T>
class ngram {

public:
    ngram() = default;

    ngram(std::vector<T> &context_, T &token_) :
            context(context_),
            token(token_) {}

    explicit ngram(const std::vector<T> &tokens) {
        for (size_t i = 0; i < tokens.size() - 1; ++i) {
            context.push_back(tokens[i]);
        }
        token = tokens[tokens.size() - 1];
    }

    ngram(const std::vector<T> &context, const T &token) :
            context(context),
            token(token) {}

    ~ngram() = default;

    bool operator==(const ngram &right) const {
        for (size_t i = 0; i < context.size(); ++i) {
            if (this->context[i] != right.context[i]) {
                return false;
            }
        }
        return this->token == right.token;
    };

    ngram &operator=(const ngram &other) = default;


    [[nodiscard]] std::vector<T> getContext() const {
        return context;
    }

    [[nodiscard]] T getToken() const {
        return token;
    }

private:
    std::vector<T> context;
    T token;
};


#endif //N_GRAM_NGRAM_H
