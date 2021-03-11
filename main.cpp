#include <iostream>
#include "inc/NgramModel.h"


/*
 Todo:
    - створити клас NGRAM: контекст з n-1 попередніх слів та слово, яке передбачається
    - створити базовий препроцесінг: токенізація, видалення пунктуації, кейс і тд
    - розібратись з мапою або створити свою структуру
    - дописати методи
 */

int main() {

    std::vector<std::string> tokens;
    tokens = {"You", "will", "rejoice", "to", "hear", "that", "no", "disaster", "has", "accompanied", "the",
              "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded", "with", "such", "evil",
              "forebodings", ".", "I", "arrived", "here", "yesterday", ",", "and", "my", "first", "task", "is", "to",
              "assure", "my", "dear", "sister", "of", "my", "welfare", "and", "increasing", "confidence", "in", "the",
              "success", "of", "my", "undertaking", "."};

    NgramModel m{3};
    auto ngrams = m.get_ngrams(tokens);

    NgramModel::print_ngrams(ngrams);

    return 0;
}
