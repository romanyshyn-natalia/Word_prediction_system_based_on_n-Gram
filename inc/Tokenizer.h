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
#include "NgramModel.h"

using namespace boost::locale::boundary;

std::vector<std::string> tokenize_text(const std::string &text);

std::string read_binary_file(const std::string &filename);

void print_context(const NgramModel& model);

void print_context_counter(const NgramModel& model);


#endif //N_GRAM_TOKENIZER_H
