#ifndef N_GRAM_NGRAMMODEL_H
#define N_GRAM_NGRAMMODEL_H


#include <unordered_map>
#include "ngram_hasher.h"
#include "vector_hasher.h"


class NgramModel {
public:
    //constructors
    explicit NgramModel(size_t n_ = 1) : n{n_} { //! Objects are already created by their default constructors.
    }

    //destructor
    ~NgramModel() = default;

    //methods
    std::vector<Ngram> get_ngrams(std::vector<std::string> &tokens) const; // Possibly needs refactoring -- changes tokens

    void update(std::vector<std::string> &tokens);

    double probability(const std::vector<std::string> &current_context, std::string &token);

    std::string random_token(const std::vector<std::string> &current_context);

    std::string generate_text(size_t token_count);

    std::unordered_map<std::vector<std::string>, std::vector<std::string>, vector_hasher> context;

    std::unordered_map<Ngram, unsigned long long, ngram_hasher> ngram_count;

private:
    size_t n; //! O.F. Please, rename it to more self-documenting!
};


double random_double();

#endif //N_GRAM_NGRAMMODEL_H
