#ifndef N_GRAM_WORD_TOKENIZER_H
#define N_GRAM_WORD_TOKENIZER_H

#include <string>
#include <vector>
#include <boost/locale/conversion.hpp>
#include <boost/locale.hpp>


using namespace boost::locale::boundary;

/*
 * Reads input file into the memory.
 * @param the name of the filename.
 * @return string.
 */
std::string read_binary_file(const std::string &filename);

/*
 * Function normalizes the input text and split by words.
 * @param the string with text.
 * @return vector of strings.
 */
std::vector<std::string> tokenize_text(const std::string &text);


#endif //N_GRAM_WORD_TOKENIZER_H
