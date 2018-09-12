#include "catch/catch.hpp"
#include "pacif/token.hpp"

using namespace pacif;

TEST_CASE("Token class") {
    SECTION("literals") {
        auto tok = token::eof();
        CHECK(tok.get_kind() == token::Eof);

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_string(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);

        tok = token::loop();
        CHECK(tok.get_kind() == token::Loop);

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_string(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);

        tok = token::global();
        CHECK(tok.get_kind() == token::Global);

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_string(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);

        tok = token::stop();
        CHECK(tok.get_kind() == token::Stop);

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_string(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);
    }

    SECTION("integers") {
        auto tok = token::integer(25);
        CHECK(tok.get_kind() == token::Integer);
        CHECK(tok.as_integer() == 25);

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_string(), pacif::error);
    }

    SECTION("reals") {
        auto tok = token::real(25);
        CHECK(tok.get_kind() == token::Real);
        CHECK(tok.as_real() == 25);

        CHECK_THROWS_AS(tok.as_integer(), pacif::error);
        CHECK_THROWS_AS(tok.as_string(), pacif::error);
    }


    SECTION("strings") {
        auto tok = token::string("foo");
        CHECK(tok.get_kind() == token::String);
        CHECK(tok.as_string() == "foo");

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);
    }

    SECTION("data") {
        auto tok = token::data("foo");
        CHECK(tok.get_kind() == token::Data);
        CHECK(tok.as_string() == "foo");

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);
    }

    SECTION("save") {
        auto tok = token::save("foo");
        CHECK(tok.get_kind() == token::Save);
        CHECK(tok.as_string() == "foo");

        CHECK_THROWS_AS(tok.as_real(), pacif::error);
        CHECK_THROWS_AS(tok.as_integer(), pacif::error);
    }
}
