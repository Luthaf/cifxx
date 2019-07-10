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
        auto token = tokenizer("?").next();
        CHECK(token.kind() == token::QuestionMark);

        token = tokenizer(".").next();
        CHECK(token.kind() == token::Dot);

        token = tokenizer("").next();
        CHECK(token.kind() == token::Eof);
    }

    SECTION("keywords") {
        auto token = tokenizer("loop_").next();
        CHECK(token.kind() == token::Loop);

        token = tokenizer("stop_").next();
        CHECK(token.kind() == token::Stop);

        token = tokenizer("global_").next();
        CHECK(token.kind() == token::Global);

        token = tokenizer("data_56").next();
        CHECK(token.kind() == token::Data);
        CHECK(token.as_string() == "56");

        token = tokenizer("save_56").next();
        CHECK(token.kind() == token::Save);
        CHECK(token.as_string() == "56");
    }

    SECTION("strings") {
        auto token = tokenizer("string").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("'string'").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("'string' not this one").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("'string'but this one'").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string'but this one");

        token = tokenizer("\"string\"").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("\"string\" not here").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string");

        token = tokenizer("\"string\"but here\"").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "string\"but here");

        token = tokenizer(";foo bar\n; bar\n;").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "foo bar\n bar\n");

        token = tokenizer(";foo\n;bar\n;\n").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "foo\nbar\n");

        token = tokenizer("; foo\n;bar\n;").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == " foo\nbar\n");

        token = tokenizer("_string").next();
        CHECK(token.kind() == token::Tag);
        CHECK(token.as_string() == "_string");
        CHECK(token.as_tag() == "_string");

        CHECK_THROWS_WITH(tokenizer("$string").next(),
            "error on line 1: invalid string value '$string': "
            "'$' is not allowed as the first character of unquoted strings"
        );
        CHECK_THROWS_WITH(tokenizer("\r[string").next(),
            "error on line 2: invalid string value '[string': "
            "'[' is not allowed as the first character of unquoted strings"
        );
        CHECK_THROWS_WITH(tokenizer("\n]string").next(),
            "error on line 2: invalid string value ']string': "
            "']' is not allowed as the first character of unquoted strings"
        );
        CHECK_THROWS_WITH(tokenizer("\n\r\n]string").next(),
            "error on line 3: invalid string value ']string': "
            "']' is not allowed as the first character of unquoted strings"
        );

        token = tokenizer("'$string'").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "$string");

        token = tokenizer("'[string'").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "[string");

        token = tokenizer("']string'").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "]string");
    }

    SECTION("numbers") {
        auto token = tokenizer("42").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42);

        token = tokenizer("-33").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == -33);

        token = tokenizer("+7833").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 7833);

        token = tokenizer("42(4)").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 424);

        token = tokenizer("42(43)").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 4243);

        token = tokenizer("42(4)4").next();
        CHECK(token.kind() == token::String);
        CHECK(token.as_string() == "42(4)4");

        token = tokenizer("42.5").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.5);

        token = tokenizer("42.5(3)").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.53);

        token = tokenizer("42.5(37)").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.537);

        token = tokenizer("42.").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42.);

        token = tokenizer(".42").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == .42);

        token = tokenizer("42e6").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42e6);

        token = tokenizer("42e-8").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == 42e-8);

        token = tokenizer("-25.5").next();
        CHECK(token.kind() == token::Number);
        CHECK(token.as_number() == -25.5);

        token = tokenizer("+67.9").next();
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
        CHECK(token.as_string() == "test");

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
        CHECK(token.as_string() == "test");

        token = stream.next();
        CHECK(token.kind() == token::Data);
        CHECK(token.as_string() == "me");

        // Checking multiple calls to next at the end
        CHECK(stream.next().kind() == token::Eof);
        CHECK(stream.next().kind() == token::Eof);
    }
}
