//
// Created by nataliia on 13.03.21.
//

#ifndef N_GRAM_TOKENIZER_H
#define N_GRAM_TOKENIZER_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <boost/locale/conversion.hpp>
#include <boost/locale.hpp>

using namespace boost::locale::boundary;

std::vector<std::string> tokenize_text(const std::string &text);

std::string read_binary_file(const std::string &filename);

#endif //N_GRAM_TOKENIZER_H
