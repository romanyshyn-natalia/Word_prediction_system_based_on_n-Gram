// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <gtest/gtest.h>
#include "ngram_model.h"
#include "word_tokenizer.h"


class run_test: public ::testing::Test {
public:
    std::string test_string = "When my vision returned, I saw two smiling Sir Juffin coming toward me. "
                                  "I had met Sir Juffin Hully and frivolously accepted the strangest job offer anyone could ever imagine!";

    std::vector<unsigned long> context = {123456, 123457};
    unsigned long token = 123458;

    ngram_model<unsigned long> model;

    size_t n_grams = 3, suggestions = 2;
    double k = 1;

    std::vector<std::basic_string<char>> tokenized;

    void SetUp() override {
        auto lbm = boost::locale::localization_backend_manager::global();
        auto s = lbm.get_all_backends();
        lbm.select("icu");
        boost::locale::localization_backend_manager::global(lbm);
        boost::locale::generator g;
        std::locale::global(g(""));
        model = ngram_model<unsigned long>{n_grams, suggestions, k};
        tokenized = tokenize_text(test_string);
    }
};

    TEST_F(run_test, check_tokenizing) {
        std::string test = "Sunny day. Sunny afternoon. Sunny after rain.";
        std::vector<std::string> result = {"sunny", "day", "sunny", "afternoon", "sunny", "after", "rain"};
        auto tokenized = tokenize_text(test);
        EXPECT_EQ(tokenized, result);
    }

    TEST_F(run_test, check_ngram_1) {
        ngram<unsigned long> ngram(context, token);
        EXPECT_EQ(ngram.getContext(), context);
        EXPECT_EQ(ngram.getToken(), token);
    }

    TEST_F(run_test, check_ngram_2) {
        ngram<unsigned long> ngram(context);
        std::vector<unsigned long> result = {context.at(0)};
        EXPECT_EQ(ngram.getContext(), result);
        EXPECT_EQ(ngram.getToken(), context.at(1));
    }

    TEST_F(run_test, check_ngram_eq) {
        ngram<unsigned long> ngram_1(context, token);
        ngram<unsigned long> ngram_2(context, token);
        EXPECT_TRUE(ngram_1 == ngram_2);
    }

    TEST_F(run_test, check_ngram_model) {
        EXPECT_TRUE(model.is_empty());
        model.update(tokenized);
        EXPECT_FALSE(model.is_empty());
    }

    TEST_F(run_test, check_ngram_prob) {
        double result = 0.06896;
        model.update(tokenized);
        auto prob = model.probability({(unsigned long) std::hash<std::string>{}("met"),
                                            (unsigned long) std::hash<std::string>{}("sir")}, (unsigned long)std::hash<std::string>{}("juffin"));
        EXPECT_NEAR(result, prob, 0.001);
    }

    TEST_F(run_test, check_ngram_autocomplete) {
        std::vector<std::basic_string<char>> result = {"juffin"};
        model.update(tokenized);
        auto complete = model.autocomplete({(unsigned long) std::hash<std::string>{}("met"),
                            (unsigned long) std::hash<std::string>{}("sir")});
        EXPECT_EQ(result, complete);
    }