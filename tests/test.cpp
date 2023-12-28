#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "../tuim.hpp"

TEST_SUITE("string") {

    TEST_CASE("length") {
        CHECK(tuim::string::length("abcde") == 5);
        CHECK(tuim::string::length("12345€") == 6);
        CHECK(tuim::string::length("12ßEP😀A") == 7);
    }

    TEST_CASE("fill") {
        CHECK(tuim::string::fill("khâzad-dûm", 0) == "");
        CHECK(tuim::string::fill("22-cycle", 1) == "22-cycle");
        CHECK(tuim::string::fill("hello world", 3) == "hello worldhello worldhello world");
        CHECK(tuim::string::fill("a", 5) == "aaaaa");
    }
    
    TEST_CASE("is_style") {
        CHECK(tuim::string::is_style("&vehicle", 0));
        CHECK(tuim::string::is_style("hello #_ff00ffworld", 6));
        CHECK_FALSE(tuim::string::is_style("&computer", 1));
        CHECK_FALSE(tuim::string::is_style("l/&ion", 2));
    }
    
    TEST_CASE("extract_style") {
        // TODO: need to init tuim before testing (because it checks for style tags)
    }

    TEST_CASE("strip_styles") {
        // TODO: need to init tuim before testing (because it checks for style tags)
    }

    TEST_CASE("utf8_to_int") {
        CHECK(tuim::string::utf8_to_int("a") == 97);
        CHECK(tuim::string::utf8_to_int("£") == 49827);
        CHECK(tuim::string::utf8_to_int("æ") == 50086);
        CHECK(tuim::string::utf8_to_int("😀") == 4036991104);
        CHECK(tuim::string::utf8_to_int(tuim::string::int_to_utf8(4036991104)) == 4036991104);
    }
    
    TEST_CASE("int_to_utf8") {
        CHECK(tuim::string::int_to_utf8(97) == "a");
        CHECK(tuim::string::int_to_utf8(49827) == "£");
        CHECK(tuim::string::int_to_utf8(50086) == "æ");
        CHECK(tuim::string::int_to_utf8(4036991104) == "😀");    
        CHECK(tuim::string::int_to_utf8(tuim::string::utf8_to_int("😀")) == "😀");
    }

    TEST_CASE("is_printable") {
        CHECK(tuim::string::is_printable(tuim::string::utf8_to_int("a")));
        CHECK(tuim::string::is_printable(tuim::string::utf8_to_int("ß")));
        CHECK(tuim::string::is_printable(tuim::string::utf8_to_int("€")));
        CHECK(tuim::string::is_printable(tuim::string::utf8_to_int("😀")));
        for(int i = 0; i < 0x20; i++)
            CHECK_FALSE(tuim::string::is_printable(i));
        // CHECK_FALSE(tuim::string::is_printable(127));
        // CHECK_FALSE(tuim::string::is_printable(49792));
    }

    TEST_CASE("char_length") {
        CHECK(tuim::string::char_length('a') == 1);
        CHECK(tuim::string::char_length("é"[0]) == 2);
        CHECK(tuim::string::char_length("€"[0]) == 3);
        CHECK(tuim::string::char_length("😀"[0]) == 4);
    }

    TEST_CASE("to_lowercase") {
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("A")) == tuim::string::utf8_to_int("a"));
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("É")) == tuim::string::utf8_to_int("é"));
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("Æ")) == tuim::string::utf8_to_int("æ"));
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("Ñ")) == tuim::string::utf8_to_int("ñ"));
        
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("a")) == tuim::string::utf8_to_int("a"));
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("€")) == tuim::string::utf8_to_int("€"));
        CHECK(tuim::string::to_lowercase(tuim::string::utf8_to_int("😀")) == tuim::string::utf8_to_int("😀"));
    }

    TEST_CASE("is_alphanumeric") {
        CHECK_FALSE(tuim::string::is_alphanumeric('-'));
        CHECK_FALSE(tuim::string::is_alphanumeric('/'));
        CHECK_FALSE(tuim::string::is_alphanumeric('('));
        CHECK_FALSE(tuim::string::is_alphanumeric(']'));

        for(int c = 'a'; c <= 'z'; c++)
            CHECK(tuim::string::is_alphanumeric(c));
        for(int c = 'A'; c <= 'Z'; c++)
            CHECK(tuim::string::is_alphanumeric(c));
        for(int c = '0'; c <= '9'; c++)
            CHECK(tuim::string::is_alphanumeric(c));
        for(int c = 50048; c <= 50070; c++) // À-Ö
            CHECK(tuim::string::is_alphanumeric(c));
        for(int c = 50072; c <= 50102; c++) // Ø-ö
            CHECK(tuim::string::is_alphanumeric(c));
        for(int c = 50104; c <= 50111; c++) // ø-ÿ
            CHECK(tuim::string::is_alphanumeric(c));
    }

    TEST_CASE("is_vowel") {
        CHECK(tuim::string::is_vowel('a'));
        CHECK(tuim::string::is_vowel('e'));
        CHECK(tuim::string::is_vowel('i'));
        CHECK(tuim::string::is_vowel('o'));

        CHECK_FALSE(tuim::string::is_vowel('z'));
        CHECK_FALSE(tuim::string::is_vowel('j'));
        CHECK_FALSE(tuim::string::is_vowel('['));
        CHECK_FALSE(tuim::string::is_vowel('*'));
        // TODO: test other alphabets
    }
}

TEST_SUITE("items") {

    TEST_CASE("calc_text_width") {
        CHECK(tuim::calc_text_width("hello world") == 11);
        CHECK(tuim::calc_text_width("præss ënt€r to stärt!\n") == 22);
        CHECK(tuim::calc_text_width("#ffff00player name:&r xorrad") == 19);
        CHECK(tuim::calc_text_width("#_ffff00player name:&r xorrad") == 19);
        CHECK(tuim::calc_text_width("#ff00ff;cursor") == 6);
        CHECK(tuim::calc_text_width("#_ff00ff;cursor") == 6);
        CHECK(tuim::calc_text_width("&acursor") == 8);
        CHECK(tuim::calc_text_width("&_acursor") == 9);
    }

    TEST_CASE("calc_text_vector_width") {
        CHECK(tuim::calc_text_vector_width({"hello world", "cap", "elephant"}) == 11);
        CHECK(tuim::calc_text_vector_width({"1", "#_fffffftuim", "12"}) == 4);
        CHECK(tuim::calc_text_vector_width({"magenta", "&bstrong", "archs"}) == 7);
    }

    TEST_CASE("calc_columns_width") {
        CHECK(tuim::calc_columns_width({"a", "abc", "#ffffffabcde"}, {{"1234", "&b023", "1"}, {"#_ffffff12", "1", "12"}}) == std::vector<int>{4,3,5});
        CHECK(tuim::calc_columns_width({"a", "ab"}, {}) == std::vector<int>{1,2});
    }
}

TEST_SUITE("colors") {

    TEST_CASE("from_hex") {
        tuim::color::color c = tuim::color::from_hex("ffffff");
        CHECK(c.r == 255);
        CHECK(c.g == 255);
        CHECK(c.b == 255);
        CHECK(c.background == false);
        
        c = tuim::color::from_hex("_1050f0");
        CHECK(c.r == 16);
        CHECK(c.g == 80);
        CHECK(c.b == 240);
        CHECK(c.background == true);
    }

    TEST_CASE("to_int") {
        CHECK(tuim::color::to_int(tuim::color::color{159, 171, 8}) == 10464008);
    }
    
    TEST_CASE("to_hex") {
        CHECK(tuim::color::to_hex(tuim::color::color{255, 255, 255, false}) == "ffffff");
        CHECK(tuim::color::to_hex(tuim::color::color{16, 80, 240, true}) == "_1050f0");
        CHECK(tuim::color::to_hex(tuim::color::from_hex("_ae8f01")) == "_ae8f01");
    }

    TEST_CASE("to_int") {
        CHECK(tuim::color::to_ansi(tuim::color::color{255, 255, 255, false}) == "\033[38;2;255;255;255m");
        CHECK(tuim::color::to_ansi(tuim::color::color{10, 11, 12, true}) == "\033[48;2;10;11;12m");
    }
}

int main(int argc, char* argv[]) {
    tuim::init(argc, argv);
    printf("\033[?1049l"); // disabled alternate buffer
    tuim::native::show_user_inputs();
    tuim::native::set_cursor_visible(true);
    int res = doctest::Context(argc, argv).run();
    tuim::delete_context();
    return res;
}