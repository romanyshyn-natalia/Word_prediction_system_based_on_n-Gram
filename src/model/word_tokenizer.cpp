// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <fstream>
#include "model/word_tokenizer.h"


std::string read_binary_file(const std::string &filename) {
    std::ifstream raw_file(filename, std::ios::binary);
    if (!raw_file) {
        throw std::runtime_error("Error with opening the file.");
    }
    std::ostringstream buffer_ss;
    buffer_ss << raw_file.rdbuf();
    std::string output{buffer_ss.str()};
    return output;
}

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


std::unordered_map<unsigned long, std::string> tokens_hasher(const std::vector<std::string> &tokens) {
    std::unordered_map<unsigned long, std::string> hashed_tokens;
    for (const auto &token: tokens) {
        std::size_t token_hash = std::hash<std::string>{}(token);
        hashed_tokens[token_hash] = token;
    }
    return hashed_tokens;
}

std::vector<unsigned long> hashed_text(const std::vector<std::string> &tokenized) {
    std::vector<unsigned long> hashed;
    hashed.reserve(tokenized.size());
    for (const auto &word: tokenized) {
        hashed.emplace_back(std::hash<std::string>{}(word));
    }
    return hashed;
}