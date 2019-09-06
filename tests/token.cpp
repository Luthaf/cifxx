#include "catch/catch.hpp"
#include "cifxx/token.hpp"

using namespace cifxx;

TEST_CASE("Token class") {
    SECTION("literals") {
        auto tok = token::eof();
        CHECK(tok.kind() == token::Eof);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);

        tok = token::loop();
        CHECK(tok.kind() == token::Loop);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);

        tok = token::global();
        CHECK(tok.kind() == token::Global);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);

        tok = token::stop();
        CHECK(tok.kind() == token::Stop);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);

        tok = token::question_mark();
        CHECK(tok.kind() == token::QuestionMark);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);

        tok = token::dot();
        CHECK(tok.kind() == token::Dot);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);
    }

    SECTION("numbers") {
        auto tok = token::number(25);
        CHECK(tok.kind() == token::Number);
        CHECK(tok.as_number() == 25);

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_str_view(), cifxx::error);
    }

    SECTION("strings") {
        auto tok = token::string("foo");
        CHECK(tok.kind() == token::String);
        CHECK(tok.as_str_view() == "foo");

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
    }

    SECTION("tags") {
        auto tok = token::tag("_foo");
        CHECK(tok.kind() == token::Tag);
        CHECK(tok.as_str_view() == "_foo");
        CHECK(tok.as_tag() == "_foo");

        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
    }

    SECTION("data") {
        auto tok = token::data("foo");
        CHECK(tok.kind() == token::Data);
        CHECK(tok.as_str_view() == "foo");

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
    }

    SECTION("save") {
        auto tok = token::save("foo");
        CHECK(tok.kind() == token::Save);
        CHECK(tok.as_str_view() == "foo");

        CHECK_THROWS_AS(tok.as_tag(), cifxx::error);
        CHECK_THROWS_AS(tok.as_number(), cifxx::error);
    }
}
