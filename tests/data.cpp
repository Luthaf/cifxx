#include "catch/catch.hpp"
#include "pacif/data.hpp"
using namespace pacif;

TEST_CASE("data class") {
    auto data = pacif::data("this_is_data");

    CHECK(data.find("_foo") == data.end());

    data.insert("_float", 32);
    CHECK(data.find("_float") != data.end());
    CHECK(data.find("_float")->second.is_number());
    CHECK(data.find("_float")->second.as_number() == 32);

    data.insert("_string", "some more data");
    CHECK(data.find("_string") != data.end());
    CHECK(data.find("_string")->second.is_string());
    CHECK(data.find("_string")->second.as_string() == "some more data");

    data.insert("_vector", vector_t{value("here"), value(22)});
    CHECK(data.find("_vector") != data.end());
    CHECK(data.find("_vector")->second.is_vector());
    auto vector = data.find("_vector")->second.as_vector();
    CHECK(vector[0].is_string());
    CHECK(vector[0].as_string() == "here");
    CHECK(vector[1].is_number());
    CHECK(vector[1].as_number() == 22);

    data.insert("_override", 32);
    auto result = data.insert("_override", "32");
    CHECK(result.second == false);

    CHECK(data.find("_override") != data.end());
    CHECK(data.find("_override")->second.is_number());
    CHECK(data.find("_override")->second.as_number() == 32);
}
