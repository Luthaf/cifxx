#include "catch/catch.hpp"
#include "cifxx/tokenizer.hpp"
using namespace cifxx;

TEST_CASE("basic values parsing") {
    SECTION("digit") {
        const auto DIGITS = {
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };

        for (auto c: DIGITS) {
            CHECK(cifxx::is_digit(c));
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
            CHECK(cifxx::is_ordinary_char(c));
            CHECK(cifxx::is_non_blank_char(c));
            CHECK_FALSE(cifxx::is_whitespace(c));
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
            CHECK(cifxx::is_non_blank_char(c));
            CHECK_FALSE(cifxx::is_whitespace(c));
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
            CHECK(cifxx::is_tag_name(tag));
        }

        std::vector<std::string> NOT_TAGS = {
            "f",
            "_",
            "_à",
            "_not_ascii_é",
        };

        for (auto& tag: NOT_TAGS) {
            CHECK_FALSE(cifxx::is_tag_name(tag));
        }
    }
}

TEST_CASE("tokenizer") {
    SECTION("symbols") {
        auto tokenizer = cifxx::tokenizer("?");
        CHECK(tokenizer.next().kind() == token::QuestionMark);

        tokenizer = cifxx::tokenizer(".");
        CHECK(tokenizer.next().kind() == token::Dot);

        tokenizer = cifxx::tokenizer("");
        CHECK(tokenizer.next().kind() == token::Eof);
    }

    SECTION("keywords") {
        auto tokenizer = cifxx::tokenizer("loop_");
        CHECK(tokenizer.next().kind() == token::Loop);

        tokenizer = cifxx::tokenizer("stop_");
        CHECK(tokenizer.next().kind() == token::Stop);

        tokenizer = cifxx::tokenizer("global_");
        CHECK(tokenizer.next().kind() == token::Global);

        tokenizer = cifxx::tokenizer("data_56");
        auto token = tokenizer.next();
        CHECK(token.kind() == token::Data);
        CHECK(token.as_str_view() == "56");

        tokenizer = cifxx::tokenizer("save_56");
        token = tokenizer.next();
        CHECK(token.kind() == token::Save);
        CHECK(token.as_str_view() == "56");
    }

    SECTION("strings") {
        auto tokenizer = cifxx::tokenizer("string");
        auto token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string");

        tokenizer = cifxx::tokenizer("'string'");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string");

        tokenizer = cifxx::tokenizer("'string' not this one");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string");

        tokenizer = cifxx::tokenizer("'string'but this one'");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string'but this one");

        tokenizer = cifxx::tokenizer("\"string\"");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string");

        tokenizer = cifxx::tokenizer("\"string\" not here");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string");

        tokenizer = cifxx::tokenizer("\"string\"but here\"");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "string\"but here");

        tokenizer = cifxx::tokenizer(";foo bar\n bar\n;");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "foo bar\n bar\n");

        // missing final ';'
        tokenizer = cifxx::tokenizer(";foo\nbar\n\n");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "foo\nbar\n\n");

        tokenizer = cifxx::tokenizer("; foo\nbar\n;");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == " foo\nbar\n");

        tokenizer = cifxx::tokenizer("_string");
        token = tokenizer.next();
        CHECK(token.kind() == token::Tag);
        CHECK(token.as_str_view() == "_string");
        CHECK(token.as_tag() == "_string");

        tokenizer = cifxx::tokenizer("$string");
        CHECK_THROWS_WITH(tokenizer.next(),
            "error on line 1: invalid string value '$string': "
            "'$' is not allowed as the first character of unquoted strings"
        );

        tokenizer = cifxx::tokenizer("\r[string");
        CHECK_THROWS_WITH(tokenizer.next(),
            "error on line 2: invalid string value '[string': "
            "'[' is not allowed as the first character of unquoted strings"
        );

        tokenizer = cifxx::tokenizer("\n]string");
        CHECK_THROWS_WITH(tokenizer.next(),
            "error on line 2: invalid string value ']string': "
            "']' is not allowed as the first character of unquoted strings"
        );

        tokenizer = cifxx::tokenizer("\n\r\n]string");
        CHECK_THROWS_WITH(tokenizer.next(),
            "error on line 3: invalid string value ']string': "
            "']' is not allowed as the first character of unquoted strings"
        );

        tokenizer = cifxx::tokenizer("'$string'");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "$string");

        tokenizer = cifxx::tokenizer("'[string'");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "[string");

        tokenizer = cifxx::tokenizer("']string'");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "]string");
    }

    SECTION("numbers") {
        auto tokenizer = cifxx::tokenizer("42");
        auto token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42);

        tokenizer = cifxx::tokenizer("-33");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == -33);

        tokenizer = cifxx::tokenizer("+7833");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 7833);

        tokenizer = cifxx::tokenizer("42(4)");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 424);

        tokenizer = cifxx::tokenizer("42(43)");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 4243);

        tokenizer = cifxx::tokenizer("42(4)4");
        token = tokenizer.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "42(4)4");

        tokenizer = cifxx::tokenizer("42.5");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.5);

        tokenizer = cifxx::tokenizer("42.5(3)");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.53);

        tokenizer = cifxx::tokenizer("42.5(37)");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.537);

        tokenizer = cifxx::tokenizer("42.");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.);

        tokenizer = cifxx::tokenizer(".42");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == .42);

        tokenizer = cifxx::tokenizer("42e6");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42e6);

        tokenizer = cifxx::tokenizer("42e-8");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42e-8);

        tokenizer = cifxx::tokenizer("-25.5");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == -25.5);

        tokenizer = cifxx::tokenizer("+67.9");
        token = tokenizer.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == +67.9);
    }

    SECTION("comments") {
        auto stream = tokenizer("42.5 # comment \t\n     test\n# commment __not_a_tag");
        auto token = stream.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.5);

        token = stream.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "test");

        CHECK(stream.next().kind() == token::Eof);
    }

    SECTION("multiple tokens") {
        auto stream = tokenizer("42.5 __tag- 'test' data_me");

        auto token = stream.next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.5);

        token = stream.next();
        CHECK(token.kind() == token::Tag);
        CHECK(token.as_tag() == "__tag-");

        token = stream.next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_str_view() == "test");

        token = stream.next();
        CHECK(token.kind() == token::Data);
        CHECK(token.as_str_view() == "me");

        // Checking multiple calls to next at the end
        CHECK(stream.next().kind() == token::Eof);
        CHECK(stream.next().kind() == token::Eof);
    }
}
