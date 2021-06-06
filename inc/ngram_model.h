#ifndef N_GRAM_NGRAM_MODEL_H
#define N_GRAM_NGRAM_MODEL_H


#include <unordered_map>
#include "ngram_hasher.h"
#include "vector_hasher.h"


class ngram_model {
public:
    explicit ngram_model(size_t n_grams = 2, size_t suggestions = 1, double k_smoothing = 1) : number_of_grams{n_grams},
                                                                                               number_of_suggestions{
                                                                                                       suggestions},
                                                                                               k{k_smoothing} {}

    ~ngram_model() = default;

    /*
     * Updates model with extracted ngrams from tokens
     * @param tokens vector of strings with tokenized words.
     * @return nothing.
     */
    void update(const std::vector<std::string> &tokens);

    /*
     * Calculate probabilities of seeing given in given context.
     * @param current_context vector of string.
     * @param token string.
     * @return double.
     */
    double probability(const std::vector<std::string> &current_context, const std::string &token);


    std::vector<std::pair<std::string, double>> probable_tokens(const std::vector<std::string> &current_context);

    /*
     * Generate most probable end of phrase for user input.
     * @param user_text_tokenized vector of string.
     * @return vector of strings.
     */
    std::vector<std::string> autocomplete(const std::vector<std::string> &user_text_tokenized);

    std::unordered_map<std::vector<std::string>, std::vector<std::string>, vector_hasher> context;

    std::unordered_map<ngram, unsigned long long, ngram_hasher> ngram_count;


private:
    size_t number_of_grams;
    size_t number_of_suggestions;
    std::set<std::string> tokens_list;
    std::unordered_map<ngram, double, ngram_hasher> probability_map;
    std::unordered_map<std::string, double> tokens_count;
    double k;
};


#endif //N_GRAM_NGRAM_MODEL_H
