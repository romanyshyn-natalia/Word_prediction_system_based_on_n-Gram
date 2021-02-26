#include <iostream>
#include "inc/NgramModel.h"


int main() {

    std::vector<std::string> tokens;
    tokens = {"You", "will", "rejoice", "to", "hear", "that", "no", "disaster", "has", "accompanied", "the",
              "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded", "with", "such", "evil",
              "forebodings", ".", "I", "arrived", "here", "yesterday", ",", "and", "my", "first", "task", "is", "to",
              "assure", "my", "dear", "sister", "of", "my", "welfare", "and", "increasing", "confidence", "in", "the",
              "success", "of", "my", "undertaking", "."};

    NgramModel m{3};
    auto ngrams = m.get_ngrams(tokens);

    m.print_ngrams(ngrams);

    return 0;
}
