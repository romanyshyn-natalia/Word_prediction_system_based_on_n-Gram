#ifndef N_GRAM_NGRAM_H
#define N_GRAM_NGRAM_H

#include <vector>
#include <string>


class ngram {

public:
    ngram() = default;

    ngram(std::vector<std::string> &context, std::string &token);

    ngram(const std::vector<std::string> &context, const std::string &token);

    explicit ngram(const std::vector<std::string> &tokens);

    ngram(const ngram &ngram) = default;

    ~ngram() = default;

    bool operator==(const ngram &right) const;

    ngram &operator=(const ngram &other) = default;

    std::ofstream &operator<<(std::ofstream &stream);

    [[nodiscard]] std::vector<std::string> getContext() const {
        return context;
    }

    [[nodiscard]] std::string getToken() const {
        return token;
    }

private:
    std::vector<std::string> context;
    std::string token;
};


#endif //N_GRAM_NGRAM_H
