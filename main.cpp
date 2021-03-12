#include <iostream>
#include "inc/NgramModel.h"
#include "inc/Tokenizer.h"


/*
 Todo:
    - створити клас NGRAM: контекст з n-1 попередніх слів та слово, яке передбачається
    - створити базовий препроцесінг: токенізація, видалення пунктуації, кейс і тд
    - розібратись з мапою або створити свою структуру
    - дописати методи
 */

int main() {

    // connect backend
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

//    std::vector<std::string> tokens;
//    tokens = {"You", "will", "rejoice", "to", "hear", "that", "no", "disaster", "has", "accompanied", "the",
//              "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded", "with", "such", "evil",
//              "forebodings", ".", "I", "arrived", "here", "yesterday", ",", "and", "my", "first", "task", "is", "to",
//              "assure", "my", "dear", "sister", "of", "my", "welfare", "and", "increasing", "confidence", "in", "the",
//              "success", "of", "my", "undertaking", "."};

    std::vector<std::string> tokenized;
    std::string text_data = read_binary_file("../Frankenstein.txt");
    tokenized = tokenize_text(text_data);

    NgramModel m{3};
    auto ngrams = m.get_ngrams(tokenized);

    NgramModel::print_ngrams(ngrams);

    return 0;
}
