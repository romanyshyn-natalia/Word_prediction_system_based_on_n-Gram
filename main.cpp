// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "inc/ngram_model.h"
#include "inc/word_tokenizer.h"
#include "inc/time_measuring.h"

int main() {

#define PRINT_INTERACTION
#ifdef PRINT_INTERACTION
    // connect backend
    auto lbm = boost::locale::localization_backend_manager::global();
    auto s = lbm.get_all_backends();
    lbm.select("icu");
    boost::locale::localization_backend_manager::global(lbm);
    boost::locale::generator g;
    std::locale::global(g(""));

    std::vector<std::string> tokenized;
    std::cout << "Reading the file..." << std::endl;
    std::string text_data = read_binary_file("../resources/51.txt");
    std::cout << "Tokenizing..." << std::endl;
    tokenized = tokenize_text(text_data);

    std::string str_n_grams, str_suggestions, str_k;
    size_t n_grams, suggestions;
    double k;

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

    std::cout << "Enter k for add-k smoothing (1 for Laplace): " << std::endl << "\t";
    std::getline(std::cin, str_k);
    std::cin.clear();
    try {
        k = std::stod(str_k);
    } catch (int ex) {
        std::cout << "Incorrect input." << std::endl;
    }
    if (k < 0.0 || k > 1.0) {
        std::cout << "Incorrect k for smoothing." << std::endl;
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

    ngram_model m{n_grams, suggestions, k};
    auto before = get_current_time_fenced();
    m.update(tokenized);
    auto time_to_calculate_reading = get_current_time_fenced() - before;
    std::cout << "model updating time: " << to_us(time_to_calculate_reading) << std::endl;


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