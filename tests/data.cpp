#include "catch/catch.hpp"
#include "cifxx/data.hpp"
using namespace cifxx;

TEST_CASE("basic_data class") {
    auto data = cifxx::basic_data();

    CHECK(data.empty());
    CHECK(data.size() == 0);
    CHECK(data.find("_foo") == data.end());

    data.emplace("_float", 32);
    CHECK(data.find("_float") != data.end());
    CHECK(data.find("_float")->second.is_number());
    CHECK(data.find("_float")->second.as_number() == 32);
    CHECK(data.get("_float").is_number());
    CHECK(data.get("_float").as_number() == 32);

    data.emplace("_string", "some more data");
    CHECK(data.find("_string") != data.end());
    CHECK(data.find("_string")->second.is_string());
    CHECK(data.find("_string")->second.as_string() == "some more data");
    CHECK(data.get("_string").is_string());
    CHECK(data.get("_string").as_string() == "some more data");

    data.emplace("_vector", vector_t{value("here"), value(22)});
    CHECK(data.find("_vector") != data.end());
    CHECK(data.find("_vector")->second.is_vector());
    auto vector = data.find("_vector")->second.as_vector();
    CHECK(vector[0].is_string());
    CHECK(vector[0].as_string() == "here");
    CHECK(vector[1].is_number());
    CHECK(vector[1].as_number() == 22);

    vector = data.get("_vector").as_vector();
    CHECK(vector[0].is_string());
    CHECK(vector[0].as_string() == "here");
    CHECK(vector[1].is_number());
    CHECK(vector[1].as_number() == 22);

    data.emplace("_override", 32);
    auto result = data.emplace("_override", "32");
    CHECK(result.second == false);

    CHECK(data.find("_override") != data.end());
    CHECK(data.find("_override")->second.is_number());
    CHECK(data.find("_override")->second.as_number() == 32);

    CHECK_FALSE(data.empty());
    CHECK(data.size() == 4);
}

TEST_CASE("data class") {
    auto data = cifxx::data("this_is_data");
    CHECK(data.name() == "this_is_data");

    CHECK(data.find("_foo") == data.end());

    data.emplace("_float", 32);
    CHECK(data.find("_float") != data.end());
    CHECK(data.find("_float")->second.is_number());
    CHECK(data.find("_float")->second.as_number() == 32);

    data.add_save("save_1", {});

    auto save = basic_data();
    save.emplace("_float", 55);
    save.emplace("_string", "some more data");
    data.add_save("save_2", std::move(save));

    CHECK(data.save().size() == 2);
    CHECK(data.save().find("save_1") != data.save().end());
    CHECK(data.save().find("save_1")->second.empty());
    CHECK(data.save().find("save_1")->second.size() == 0);

    CHECK(data.save().find("save_2") != data.save().end());
    auto& save_2 = data.save().find("save_2")->second;
    CHECK(save_2.size() == 2);
    CHECK(save_2.find("_float")->second.as_number() == 55);
    CHECK(save_2.find("_string")->second.as_string() == "some more data");
}
