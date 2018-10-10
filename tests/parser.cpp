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

        auto looped = get(blocks[0], "_looped").as_vector();
        CHECK(looped.size() == 3);
        CHECK(looped[0].as_number() == 1);
        CHECK(looped[1].as_number() == 2);
        CHECK(looped[2].as_number() == 3);

        auto changing_type = get(blocks[0], "_changing_type").as_vector();
        CHECK(changing_type.size() == 3);
        CHECK(changing_type[0].as_string() == "fe");
        CHECK(changing_type[1].as_number() == 4);
        CHECK(changing_type[2].as_string() == "zn");
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

TEST_CASE("Actual CIF files") {
    SECTION("Crystalographic CIF file") {
        std::ifstream file(DATADIR "it023_br.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        auto block = blocks[0];
        CHECK(block.name() == "IT023_BR_phase_");
        CHECK(get(block, "_cell_length_a").as_number() == 17.08321);
        CHECK(get(block, "_cell_length_b").as_number() == 17.0832);

        auto atom_site_fract_x = get(block, "_atom_site_fract_x").as_vector();
        CHECK(atom_site_fract_x.size() == 7);
        CHECK(atom_site_fract_x[0].as_number() == 0.5);
        CHECK(atom_site_fract_x[1].as_number() == 0.37563);
        CHECK(atom_site_fract_x[2].as_number() == 0.36452);
    }

    SECTION("From the COD database") {
        std::ifstream file(DATADIR "1544173.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        auto block = blocks[0];
        CHECK(block.name() == "1544173");
        CHECK(get(block, "_chemical_formula_sum").as_string() == "C20 H28 O2");
        CHECK(get(block, "_cell_length_b").as_number() == 11.50306);

        auto atom_site_fract_x = get(block, "_atom_site_fract_x").as_vector();
        CHECK(atom_site_fract_x.size() == 50);
        CHECK(atom_site_fract_x[0].as_number() == 0.2069115);
        CHECK(atom_site_fract_x[22].as_number() == 0.4594);
    }

    SECTION("From the PDBX database") {
        std::ifstream file(DATADIR "4hhb.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        auto block = blocks[0];
        CHECK(block.name() == "4HHB");
        CHECK(get(block, "_audit_conform.dict_name").as_string() == "mmcif_pdbx.dic");

        CHECK(get(block, "_cell.length_a").as_number() == 63.150);
        CHECK(get(block, "_cell.Z_PDB").as_number() == 4);
        CHECK(get(block, "_cell.pdbx_unique_axis").is_missing());


        auto x = get(block, "_atom_site.Cartn_x").as_vector();
        CHECK(x.size() == 4779);
        CHECK(x[22].as_number() == 15.048);
        CHECK(x[150].as_number() == 22.302);
    }
}
