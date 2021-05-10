#ifndef N_GRAM_NGRAMMODEL_H
#define N_GRAM_NGRAMMODEL_H


#include <unordered_map>
#include "ngram_hasher.h"
#include "vector_hasher.h"


class NgramModel {
public:
    //constructors
    explicit NgramModel(size_t n_grams = 2, size_t suggestions = 1) : number_of_grams{n_grams},
                                                                      number_of_suggestions{suggestions} {}

    //destructor
    ~NgramModel() = default;

    //methods
    void get_ngrams(const std::vector<std::string> &tokens);

    void update(const std::vector<std::string> &tokens);

    double probability(const std::vector<std::string> &current_context, const std::string &token);

    std::vector<std::pair<std::string, double>> probable_tokens(const std::vector<std::string> &current_context);

    std::vector<std::string> autocomplete(const std::vector<std::string> &user_text_tokenized);

    std::unordered_map<std::vector<std::string>, std::vector<std::string>, vector_hasher> context;

    std::unordered_map<Ngram, unsigned long long, ngram_hasher> ngram_count;

    void setNgramList(const std::vector<Ngram> &this_ngram_list) {
        ngram_list = this_ngram_list;
    }

    std::vector<Ngram> getNgramList() const {
        return ngram_list;
    }

    std::vector<Ngram> getNgramList() {
        return ngram_list;
    }

    void setProbabilityMap(const std::unordered_map<Ngram, double, ngram_hasher> &this_probability_map) {
        probability_map = this_probability_map;
    }

    std::unordered_map<Ngram, double, ngram_hasher> setProbabilityMap() const {
        return probability_map;
    }

    std::unordered_map<Ngram, double, ngram_hasher> getProbabilityMap() {
        return probability_map;
    }

private:
    size_t number_of_grams;
    size_t number_of_suggestions;
    std::vector<Ngram> ngram_list;
    std::unordered_map<Ngram, double, ngram_hasher> probability_map;
};


#endif //N_GRAM_NGRAMMODEL_H
