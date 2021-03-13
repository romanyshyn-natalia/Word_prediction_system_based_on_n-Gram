//
// Created by nataliia on 12.03.21.
//
#include "../inc/Tokenizer.h"

std::vector<std::string> tokenize_text(const std::string &text) {
    std::vector<std::string> tokens;
    std::string normalized = boost::locale::normalize(text);
    std::string cased = boost::locale::fold_case(normalized);
    ssegment_index map(word, cased.begin(), cased.end());
    map.rule(word_any);
    for (ssegment_index::iterator it = map.begin(), e = map.end(); it != e; ++it)
        tokens.push_back(*it);
    return tokens;
}


std::string read_binary_file(const std::string &filename) {
    std::ifstream raw_file(filename, std::ios::binary);
    std::ostringstream buffer_ss;
    buffer_ss << raw_file.rdbuf();
    std::string output{buffer_ss.str()};
    return output;
}

template<typename K, typename V>
void print_map(std::unordered_map<K, V> const &m) {
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

void print_context(const NgramModel& model){
    std::cout << "{[";
    for (auto const &pair: model.context) {
        for (const auto &word: pair.first.context) {
            std::cout << word << " ";
        }
        std::cout << "]: [";
        for (auto const &word: pair.second) {
            std::cout << word << ", ";
        }
        std::cout << "]," << std::endl;
    }
}

void print_context_counter(const NgramModel& model){
    std::cout << "{[";
    for (auto const &pair: model.ngram_count) {
        for (const auto &word: pair.first.context) {
            std::cout << word << " ";
        }
        std::cout << "]: " << pair.second;
        std::cout << "," << std::endl;
    }
}