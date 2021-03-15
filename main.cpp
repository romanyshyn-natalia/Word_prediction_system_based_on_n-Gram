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


    std::vector<std::string> tokens1 = {"You", "will", "rejoice", "to", "hear", "that", "no", "disaster", "has",
                                        "accompanied", "the",
                                        "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded",
                                        "with", "such", "evil",
                                        "forebodings", "You", "will", "cry", "and", "scream", "that", "no", "disaster",
                                        "has", "accompanied", "the",
                                        "commencement", "of", "an", "enterprise", "which", "you", "have", "regarded",
                                        "with", "such", "evil",
                                        "forebodings", "You", "will", "sleep", "You", "will", "eat", "You", "will",
                                        "cry"};

    std::vector<std::string> tokenized;
    std::string text_data = read_binary_file("../Frankenstein.txt");
    tokenized = tokenize_text(text_data);

    NgramModel m{3};
    m.update(tokenized);
    std::cout << m.generate_text(50) << std::endl;


    return 0;
}
