#include "catch/catch.hpp"
#include "pacif.hpp"

TEST_CASE("basic values parsing") {
    SECTION("digit") {
        const auto DIGITS = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };

        for (auto c: DIGITS) {
            CHECK(pacif::parsing::is_digit(c));
        }
    }

    SECTION("ordinary char") {
        const auto ORDINARY_CHARS = {
            '!', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1',
            '2', '3', '4', '5', '6', '7', '8', '9', ':', '<', '=', '>', '?',
            '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
            'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
            'Z', '\\', '^', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
            'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '{', '|', '}', '~'
        };

        for (auto c: ORDINARY_CHARS) {
            CHECK(pacif::parsing::is_ordinary_char(c));
            CHECK(pacif::parsing::is_non_blank_char(c));
            CHECK_FALSE(pacif::parsing::is_whitespace(c));
        }
    }

    SECTION("non blank char") {
        const auto NON_BLANK_CHARS = {
            '!', '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1',
            '2', '3', '4', '5', '6', '7', '8', '9', ':', '<', '=', '>', '?',
            '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
            'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
            'Z', '\\', '^', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
            'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '{', '|',  '}', '~', '"', '\'', '$', '#', '_',
            ';', '[', ']'
        };

        for (auto c: NON_BLANK_CHARS) {
            CHECK(pacif::parsing::is_non_blank_char(c));
            CHECK_FALSE(pacif::parsing::is_whitespace(c));
        }
    }

    SECTION("tags") {
        std::vector<std::string> TAGS = {
            "_f",
            "_foo",
            "_symmetry_space_group_name_H-M",
            "_;;;$$$$######'''''",
        };

        for (auto& tag: TAGS) {
            CHECK(pacif::parsing::is_tag_name(tag));
        }

        std::vector<std::string> NOT_TAGS = {
            "f",
            "_",
            "_à",
            "_not_ascii_é",
        };

        for (auto& tag: NOT_TAGS) {
            CHECK_FALSE(pacif::parsing::is_tag_name(tag));
        }
    }
}

TEST_CASE("tokenizer") {
    using namespace pacif::parsing;

    SECTION("keywords") {
        auto token = parser("loop_").next();
        CHECK(token.get_kind() == token::Loop);

        token = parser("stop_").next();
        CHECK(token.get_kind() == token::Stop);

        token = parser("global_").next();
        CHECK(token.get_kind() == token::Global);

        token = parser("data_56").next();
        CHECK(token.get_kind() == token::Data);
        CHECK(token.as_string() == "56");

        token = parser("save_56").next();
        CHECK(token.get_kind() == token::Save);
        CHECK(token.as_string() == "56");
    }

    SECTION("strings") {
        auto token = parser("string").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = parser("'string'").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = parser("'string' not this one").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = parser("'string'but this one'").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string'but this one");

        token = parser("\"string\"").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = parser("\"string\" not here").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = parser("\"string\"but here\"").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string\"but here");
    }

    SECTION("integers") {
        auto token = parser("42").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 42);

        token = parser("-33").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == -33);

        token = parser("+7833").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 7833);

        token = parser("42(4)").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 424);

        token = parser("42(4)4").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "42(4)4");

        token = parser("9223372036854775807").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 9223372036854775807);

        CHECK_THROWS_AS(parser("9223372036854775808").next(), pacif::error);
    }

    SECTION("reals") {
        auto token = parser("42.5").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.5);

        token = parser("42.5(3)").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.53);

        token = parser("42.").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.);

        token = parser(".42").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == .42);


        token = parser("42e6").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42e6);

        token = parser("42e-8").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42e-8);

        token = parser("-25.5").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == -25.5);

        token = parser("+67.9").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == +67.9);
    }
}
