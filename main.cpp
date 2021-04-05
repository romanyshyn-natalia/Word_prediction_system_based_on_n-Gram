// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inc/NgramModel.h"
#include "inc/word_tokenizer.h"

int main() {

#define PRINT_INTERACTION


    // connect backend
    auto lbm = boost::locale::localization_backend_manager::global();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    std::vector<std::string> tokenized;
#ifdef PRINT_INTERACTION
    std::cout << "Reading the file..." << std::endl;
#endif //PRINT_INTERACTION
    std::string text_data = read_binary_file("../resources/frankenstein.txt");
#ifdef PRINT_INTERACTION
    std::cout << "Tokenizing..." << std::endl;
#endif //PRINT_INTERACTION
    tokenized = tokenize_text(text_data);
#ifdef PRINT_INTERACTION
    std::string str_n_grams, str_suggestions;
    size_t n_grams, suggestions;
    std::cout << "Enter number of grams: " << std::endl << "\t";
    std::getline(std::cin, str_n_grams);
    std::cin.clear();
    try {
        n_grams = std::stoi(str_n_grams);

    } catch (int ex) {
        std::cout << "Incorrect input." << std::endl;
    }
    if (n_grams < 2) {
        std::cout << "Incorrect number of grams." << std::endl;
    }

    std::cout << "Enter number of suggestions: " << std::endl << "\t";
    std::getline(std::cin, str_suggestions);
    try {
        suggestions = std::stoi(str_suggestions);

    } catch (int ex) {
        std::cout << "Incorrect input." << std::endl;
    }
    if (suggestions < 1) {
        std::cout << "Incorrect number of suggestions." << std::endl;
    }
    std::cout << "Analyzing your input..." << std::endl;

    NgramModel m{n_grams, suggestions};
    m.update(tokenized);


    NgramModel bigrams{2, suggestions};
    if (n_grams == 2) {
        bigrams = m;
    } else {
        bigrams.update(tokenized);
    }

    std::vector<std::string> user_text_tokenized;

    std::string user_input;

    std::cout << "Start typing your text and when you want us to suggest something, press Enter." << std::endl;
    std::cout << "If you want to stop the program, type q." << std::endl;

    std::getline(std::cin, user_input);

    std::vector<std::string> current_input = tokenize_text(user_input);
    user_text_tokenized.reserve(user_text_tokenized.size() + std::distance(current_input.begin(),
                                                                           current_input.end()));
    user_text_tokenized.insert(user_text_tokenized.end(), current_input.begin(), current_input.end());

    while (true) {
        auto res = m.autocomplete(user_text_tokenized);

        int count = 1;
        for (const auto &elem: res) {
            std::cout << count << ". " << elem << std::endl;
            count++;
        }

        std::string str_suggestion_idx;
        int suggestion_idx;
        std::cout << "Enter number of suggestion you would like to choose:" << std::endl << "\t";
        std::getline(std::cin, str_suggestion_idx);
        if (str_suggestion_idx == "q") {
            break;
        }
        try {
            suggestion_idx = std::stoi(str_suggestion_idx);
        } catch (int ex) {
            std::cout << "Incorrect input." << std::endl;
        }
        if (1 > suggestion_idx || suggestion_idx > static_cast<int>(res.size())) {
            std::cout << "Incorrect suggestion index." << std::endl;
        }
        user_text_tokenized.emplace_back(res[suggestion_idx - 1]);
        for (const auto &word: user_text_tokenized) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
#endif //PRINT_INTERACTION
    return 0;
}