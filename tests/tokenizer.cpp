#include "catch/catch.hpp"
#include "pacif/tokenizer.hpp"
using namespace pacif;

TEST_CASE("basic values parsing") {
    SECTION("digit") {
        const auto DIGITS = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };

        for (auto c: DIGITS) {
            CHECK(pacif::is_digit(c));
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
            CHECK(pacif::is_ordinary_char(c));
            CHECK(pacif::is_non_blank_char(c));
            CHECK_FALSE(pacif::is_whitespace(c));
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
            CHECK(pacif::is_non_blank_char(c));
            CHECK_FALSE(pacif::is_whitespace(c));
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
            CHECK(pacif::is_tag_name(tag));
        }

        std::vector<std::string> NOT_TAGS = {
            "f",
            "_",
            "_à",
            "_not_ascii_é",
        };

        for (auto& tag: NOT_TAGS) {
            CHECK_FALSE(pacif::is_tag_name(tag));
        }
    }
}

TEST_CASE("tokenizer") {
    SECTION("symbols") {
        auto token = tokenizer("?").next();
        CHECK(token.get_kind() == token::QuestionMark);

        token = tokenizer(".").next();
        CHECK(token.get_kind() == token::Dot);

        token = tokenizer("").next();
        CHECK(token.get_kind() == token::Eof);
    }

    SECTION("keywords") {
        auto token = tokenizer("loop_").next();
        CHECK(token.get_kind() == token::Loop);

        token = tokenizer("stop_").next();
        CHECK(token.get_kind() == token::Stop);

        token = tokenizer("global_").next();
        CHECK(token.get_kind() == token::Global);

        token = tokenizer("data_56").next();
        CHECK(token.get_kind() == token::Data);
        CHECK(token.as_string() == "56");

        token = tokenizer("save_56").next();
        CHECK(token.get_kind() == token::Save);
        CHECK(token.as_string() == "56");
    }

    SECTION("strings") {
        auto token = tokenizer("string").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("'string'").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("'string' not this one").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("'string'but this one'").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string'but this one");

        token = tokenizer("\"string\"").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("\"string\" not here").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("\"string\"but here\"").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "string\"but here");

        token = tokenizer(";foo bar\n; bar\n;").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "foo bar\n bar\n");

        token = tokenizer(";foo\n;bar\n;\n").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "foo\nbar\n");

        token = tokenizer("; foo\n;bar\n;").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == " foo\nbar\n");

        token = tokenizer("_string").next();
        CHECK(token.get_kind() == token::Tag);
        CHECK(token.as_string() == "_string");
        CHECK(token.as_tag() == "_string");
    }

    SECTION("integers") {
        auto token = tokenizer("42").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 42);

        token = tokenizer("-33").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == -33);

        token = tokenizer("+7833").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 7833);

        token = tokenizer("42(4)").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 424);

        token = tokenizer("42(4)4").next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "42(4)4");

        token = tokenizer("9223372036854775807").next();
        CHECK(token.get_kind() == token::Integer);
        CHECK(token.as_integer() == 9223372036854775807);

        CHECK_THROWS_AS(tokenizer("9223372036854775808").next(), pacif::error);
    }

    SECTION("reals") {
        auto token = tokenizer("42.5").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.5);

        token = tokenizer("42.5(3)").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.53);

        token = tokenizer("42.").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.);

        token = tokenizer(".42").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == .42);


        token = tokenizer("42e6").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42e6);

        token = tokenizer("42e-8").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42e-8);

        token = tokenizer("-25.5").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == -25.5);

        token = tokenizer("+67.9").next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == +67.9);
    }

    SECTION("comments") {
        auto stream = tokenizer("42.5 # comment \t\n     test\n# commment __not_a_tag");
        auto token = stream.next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.5);

        token = stream.next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "test");

        CHECK(stream.next().get_kind() == token::Eof);
    }

    SECTION("multiple tokens") {
        auto stream = tokenizer("42.5 __tag- 'test' data_me");

        auto token = stream.next();
        CHECK(token.get_kind() == token::Real);
        CHECK(token.as_real() == 42.5);

        token = stream.next();
        CHECK(token.get_kind() == token::Tag);
        CHECK(token.as_tag() == "__tag-");

        token = stream.next();
        CHECK(token.get_kind() == token::String);
        CHECK(token.as_string() == "test");

        token = stream.next();
        CHECK(token.get_kind() == token::Data);
        CHECK(token.as_string() == "me");

        // Checking multiple calls to next at the end
        CHECK(stream.next().get_kind() == token::Eof);
        CHECK(stream.next().get_kind() == token::Eof);
    }
}
