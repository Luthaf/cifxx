#include <fstream>

#include "catch/catch.hpp"
#include "pacif/parser.hpp"
using namespace pacif;

static value get(const data& block, const std::string& key) {
    auto it = block.find(key);
    if (it != block.end()) {
        return it->second;
    } else {
        throw pacif::error("missing data for key " + key);
    }
}

TEST_CASE("Parse files") {
    SECTION("minimal") {
        std::ifstream file(DATADIR "minimal.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        CHECK(blocks[0].name() == "minimal");
    }

    SECTION("tags") {
        std::ifstream file(DATADIR "tags.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        CHECK(blocks[0].name() == "tags");
        CHECK(get(blocks[0], "_string").as_string() == "value");
        CHECK(get(blocks[0], "_real").as_number() == 3.252);
        CHECK(get(blocks[0], "_integer").as_number() == 42);
        CHECK(get(blocks[0], "_long_string").as_string() == " test here\n for a long string\n");
        CHECK(get(blocks[0], "_other_line").as_number() == 25);
    }

    SECTION("multiple") {
        std::ifstream file(DATADIR "multiple_data.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 2);

        CHECK(blocks[0].name() == "first");
        CHECK(get(blocks[0], "_tag1").as_number() == 1);

        CHECK(blocks[1].name() == "second");
        CHECK(get(blocks[1], "_tag2").as_number() == 3);
    }
}
