#include "catch/catch.hpp"
#include "pacif/value.hpp"
using namespace pacif;

TEST_CASE("value class") {
    SECTION("Missing") {
        auto missing = value::missing();
        REQUIRE(missing.is_missing());

        CHECK(missing.kind() == value::Missing);

        CHECK_THROWS_AS(missing.as_real(), pacif::error);
        CHECK_THROWS_AS(missing.as_string(), pacif::error);
        CHECK_THROWS_AS(missing.as_vector(), pacif::error);
    }

    SECTION("Reals") {
        auto real = value(42.0);
        REQUIRE(real.is_real());
        CHECK(real.as_real() == 42.0);

        real = value(42);
        REQUIRE(real.is_real());
        CHECK(real.as_real() == static_cast<pacif::real_t>(42));

        CHECK(real.kind() == value::Real);

        CHECK_THROWS_AS(real.as_string(), pacif::error);
        CHECK_THROWS_AS(real.as_vector(), pacif::error);
    }

    SECTION("Strings") {
        auto string = value("foobar");
        REQUIRE(string.is_string());
        CHECK(string.as_string() == "foobar");

        CHECK(string.kind() == value::String);

        CHECK_THROWS_AS(string.as_real(), pacif::error);
        CHECK_THROWS_AS(string.as_vector(), pacif::error);
    }


    SECTION("Vec") {
        auto vector = value(vector_t{value("foobar"), value(22)});
        REQUIRE(vector.is_vector());
        CHECK(vector.as_vector()[0].as_string() == "foobar");
        CHECK(vector.as_vector()[1].as_real() == 22);

        CHECK(vector.kind() == value::Vector);

        CHECK_THROWS_AS(vector.as_string(), pacif::error);
        CHECK_THROWS_AS(vector.as_real(), pacif::error);
    }
}
