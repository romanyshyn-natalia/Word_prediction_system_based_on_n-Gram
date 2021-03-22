#ifndef N_GRAM_NGRAMMODEL_H
#define N_GRAM_NGRAMMODEL_H


#include <unordered_map>
#include "ngram_hasher.h"
#include "vector_hasher.h"


class NgramModel {
public:
    //constructors
    explicit NgramModel(size_t n_ = 1) : number_of_grams{n_} {}

    //destructor
    ~NgramModel() = default;

    //methods
    std::vector<Ngram>
    get_ngrams(const std::vector<std::string> &tokens) const; // Possibly needs refactoring -- changes tokens

    void update(const std::vector<std::string> &tokens);

    double probability(const std::vector<std::string> &current_context, const std::string &token);

    std::string random_token(const std::vector<std::string> &current_context);

    std::string generate_text(const size_t &token_count);

    std::unordered_map<std::vector<std::string>, std::vector<std::string>, vector_hasher> context;

    std::unordered_map<Ngram, unsigned long long, ngram_hasher> ngram_count;

private:
    size_t number_of_grams;
};


double random_double();

#endif //N_GRAM_NGRAMMODEL_H
