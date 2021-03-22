// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inc/NgramModel.h"
#include "inc/word_tokenizer.h"


int main() {

    // connect backend
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));


//    std::vector<std::string> tokens1 = {"You", "will", "rejoice", "to", "hear", "that", "no", "disaster", "has",
//                                        "accompanied", "the",
//                                        "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded",
//                                        "with", "such", "evil",
//                                        "forebodings", "You", "will", "cry", "and", "scream", "that", "no", "disaster",
//                                        "has", "accompanied", "the",
//                                        "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded",
//                                        "with", "such", "evil",
//                                        "forebodings", "You", "will", "sleep", "You", "will", "eat", "You", "will",
//                                        "cry"};

    std::vector<std::string> tokens1 = {"You", "will", "rejoice", "You", "will", "cry",  "You", "will", "sleep", "You", "will", "eat", "You", "will",
                                        "cry"};
    std::vector<std::string> tokenized;
    std::string text_data = read_binary_file("../Frankenstein.txt");
    tokenized = tokenize_text(text_data);

    NgramModel m{3};
    m.update(tokens1);

    std::vector<std::string> v {"You",  "will"};
    std::string r("cry");
    std::cout << m.probability(v, r) << std::endl;


    return 0;
}
