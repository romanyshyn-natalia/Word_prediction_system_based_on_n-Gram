#ifndef N_GRAM_NGRAM_H
#define N_GRAM_NGRAM_H

#include <vector>
#include <string>


class Ngram {

public:
    Ngram() = default;

    Ngram(std::vector<std::string> &context, std::string &token);

    Ngram(const std::vector<std::string> &context, const std::string &token);

    explicit Ngram(const std::vector<std::string> &tokens);

    Ngram(const Ngram &ngram) = default;

    ~Ngram() = default;

    bool operator==(const Ngram &right) const;

    Ngram &operator=(const Ngram &other) = default;

    std::ofstream& operator<<(std::ofstream &stream);

    std::vector<std::string> getContext() const {
        return context;
    }

    std::string getToken() const {
        return token;
    }

private:
    std::vector<std::string> context;
    std::string token;
};


#endif //N_GRAM_NGRAM_H
