#include "catch/catch.hpp"
#include "pacif.hpp"

TEST_CASE("data class") {
    auto data = pacif::data("this_is_data");

    CHECK(data.find("_foo") == data.end());

    data.insert("_float", 32);
    CHECK(data.find("_float") != data.end());
    CHECK(data.find("_float")->second.is_real());
    CHECK(data.find("_float")->second.as_real() == 32);

    data.insert("_string", "some more data");
    CHECK(data.find("_string") != data.end());
    CHECK(data.find("_string")->second.is_string());
    CHECK(data.find("_string")->second.as_string() == "some more data");

    data.insert("_vec", pacif::vec_t{pacif::value("here"), pacif::value(22)});
    CHECK(data.find("_vec") != data.end());
    CHECK(data.find("_vec")->second.is_vec());
    auto vec = data.find("_vec")->second.as_vec();
    CHECK(vec[0].is_string());
    CHECK(vec[0].as_string() == "here");
    CHECK(vec[1].is_real());
    CHECK(vec[1].as_real() == 22);

    data.insert("_override", 32);
    auto result = data.insert("_override", "32");
    CHECK(result.second == false);

    CHECK(data.find("_override") != data.end());
    CHECK(data.find("_override")->second.is_real());
    CHECK(data.find("_override")->second.as_real() == 32);
}
