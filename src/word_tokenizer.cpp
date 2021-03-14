//
// Created by nataliia on 12.03.21.
//
#include "../inc/word_tokenizer.h"

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


