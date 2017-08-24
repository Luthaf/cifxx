#include "catch/catch.hpp"
#include "pacif.hpp"

TEST_CASE("value class") {
    SECTION("Missing") {
        auto missing = pacif::value();
        REQUIRE(missing.is_missing());

        CHECK(missing.get_kind() == pacif::value::kind::missing);

        CHECK_THROWS_AS(missing.as_real(), pacif::error);
        CHECK_THROWS_AS(missing.as_string(), pacif::error);
        CHECK_THROWS_AS(missing.as_vec(), pacif::error);
    }

    SECTION("Reals") {
        auto real = pacif::value(42.0);
        REQUIRE(real.is_real());
        CHECK(real.as_real() == 42.0);

        real = pacif::value(42.0f);
        REQUIRE(real.is_real());
        CHECK(real.as_real() == static_cast<pacif::real_t>(42.0f));

        real = pacif::value(42);
        REQUIRE(real.is_real());
        CHECK(real.as_real() == static_cast<pacif::real_t>(42));

        CHECK(real.get_kind() == pacif::value::kind::real);

        CHECK_THROWS_AS(real.as_string(), pacif::error);
        CHECK_THROWS_AS(real.as_vec(), pacif::error);
    }

    SECTION("Strings") {
        auto string = pacif::value("foobar");
        REQUIRE(string.is_string());
        CHECK(string.as_string() == "foobar");

        CHECK(string.get_kind() == pacif::value::kind::string);

        CHECK_THROWS_AS(string.as_real(), pacif::error);
        CHECK_THROWS_AS(string.as_vec(), pacif::error);
    }


    SECTION("Vec") {
        auto vec = pacif::value(pacif::vec_t{pacif::value("foobar"), pacif::value(22)});
        REQUIRE(vec.is_vec());
        CHECK(vec.as_vec()[0].as_string() == "foobar");
        CHECK(vec.as_vec()[1].as_real() == 22);

        CHECK(vec.get_kind() == pacif::value::kind::vec);

        CHECK_THROWS_AS(vec.as_string(), pacif::error);
        CHECK_THROWS_AS(vec.as_real(), pacif::error);
    }
}
