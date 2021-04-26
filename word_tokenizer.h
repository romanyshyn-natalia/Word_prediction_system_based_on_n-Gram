#ifndef WORD_TOKENIZER_H
#define WORD_TOKENIZER_H

#include <string>
#include <vector>
#include <boost/locale/conversion.hpp>
#include <boost/locale.hpp>

using namespace boost::locale::boundary;


std::vector<std::string> tokenize_text(const std::string &text);

std::string read_binary_file(const std::string &filename);


#endif // WORD_TOKENIZER_H
