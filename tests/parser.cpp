#include <fstream>

#include "catch/catch.hpp"
#include "cifxx/parser.hpp"
using namespace cifxx;

static value get(const basic_data& data, const std::string& key) {
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    } else {
        throw cifxx::error("missing data for key " + key);
    }
}

TEST_CASE("Parse files") {
    SECTION("minimal") {
        std::ifstream file(DATADIR "minimal.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        CHECK(blocks[0].name() == "minimal");
        CHECK(blocks[0].save().empty());
    }

    SECTION("multiple data blocks") {
        std::ifstream file(DATADIR "multiple_data.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 2);

        CHECK(blocks[0].name() == "first");
        CHECK(blocks[0].save().empty());
        CHECK(get(blocks[0], "_tag1").as_number() == 1);

        CHECK(blocks[1].name() == "second");
        CHECK(blocks[1].save().empty());
        CHECK(get(blocks[1], "_tag2").as_number() == 3);
    }

    SECTION("Basic usage") {
        std::ifstream file(DATADIR "basic.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        CHECK(blocks[0].name() == "tags");
        CHECK(get(blocks[0], "_string").as_string() == "value");
        CHECK(get(blocks[0], "_real").as_number() == 3.252);
        CHECK(get(blocks[0], "_integer").as_number() == 42);
        CHECK(get(blocks[0], "_long_string").as_string() == " test here\n for a long string\n");
        CHECK(get(blocks[0], "_next_line").as_number() == 25);
        CHECK(get(blocks[0], "_next_line_comment").as_string() == "str");
        CHECK(get(blocks[0], "_hash_in_str").as_string() == "s#t#r#");

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

        CHECK(get(blocks[0], "_single_quote").as_string() == "data");
        CHECK(get(blocks[0], "_single_quote2").as_string() == "a dog's life");
        CHECK(get(blocks[0], "_unicode_quoted").as_string() == "ℵ ⏣ ॵ 🤔 em: non-break: 45");
        CHECK(get(blocks[0], "_substitutions").as_string() == "\\a \\'e \\%a Csp^3^ \\\\ddb");
    }
}

TEST_CASE("Invalid CIF files") {
    auto parser = cifxx::parser(std::ifstream(DATADIR "bad/no-data.cif"));
    CHECK_THROWS_WITH(parser.parse(),
        "error on line 3: expected 'data_' at the begining of the data block, got '_tag1'"
    );

    parser = cifxx::parser(std::ifstream(DATADIR "bad/empty.cif"));
    CHECK_THROWS_WITH(parser.parse(),
        "error on line 4: expected a value for tag _tag1 , got <eof>"
    );

    parser = cifxx::parser(std::ifstream(DATADIR "bad/global.cif"));
    CHECK_THROWS_WITH(parser.parse(),
        "error on line 3: expected 'data_' at the begining of the data block, got 'global_'"
    );
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

TEST_CASE("Problematic CIF files") {
    SECTION("Additional loop_") {
        std::ifstream file(DATADIR "weird-loops.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        auto block = blocks[0];
        CHECK(block.name() == "quartz");

        // There is an additional loop_ before _cell_length_a
        CHECK(get(block, "_cell_length_a").as_vector()[0].as_number() == 4.916);
        CHECK(get(block, "_cell_length_b").as_number() == 4.916);

        CHECK(get(block, "_space_group_symop_operation_xyz").as_vector().size() == 6);
    }

    SECTION("Repeated tag in loop") {
        std::ifstream file(DATADIR "repeat-loop-tag.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);

        auto block = blocks[0];
        CHECK(block.name() == "VESTA_phase_1");

        auto labels = get(block, "_atom_site_label").as_vector();
        CHECK(labels.size() == 224);
        CHECK(labels[0].as_string() == "H");
        CHECK(labels[110].as_string() == "C");
        CHECK(labels[205].as_string() == "O");
        CHECK(labels[221].as_string() == "Zn");
    }

    SECTION("Missing data") {
        std::ifstream file(DATADIR "missing-data.cif");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 5);

        auto block = blocks[1];
        CHECK(block.name() == "sm_isp_SD0308014-standardized_unitcell");

        CHECK(get(block, "_symmetry_Int_Tables_number").as_number() == 151);
        CHECK(get(block, "_sm_cell_transformation").as_string() == "No transformation from published to standardized cell parameters necessary.\n");

        auto frac_x = get(block, "_atom_site_fract_x").as_vector();
        CHECK(frac_x.size() == 5);
        CHECK(frac_x[0].as_number() == 0.11111);
        CHECK(frac_x[2].as_number() == 0.44445);

        auto numbers = get(block, "_sm_coordination_number").as_vector();
        CHECK(numbers.size() == 5);
        CHECK(numbers[0].is_missing());
        CHECK(numbers[2].is_missing());

        auto environment = get(block, "_sm_atomic_environment_type").as_vector();
        CHECK(environment.size() == 5);
        CHECK(environment[0].as_string() == "?");
        CHECK(environment[2].as_string() == "?");

        block = blocks[2];
        CHECK(block.name() == "sm_isp_SD0308014-published_cell");

        frac_x = get(block, "_atom_site_fract_x").as_vector();
        CHECK(frac_x.size() == 1);
        CHECK(frac_x[0].is_missing());

        numbers = get(block, "_sm_coordination_number").as_vector();
        CHECK(numbers.size() == 1);
        CHECK(numbers[0].is_missing());

        environment = get(block, "_sm_atomic_environment_type").as_vector();
        CHECK(environment.size() == 1);
        CHECK(environment[0].is_missing());
    }
}

TEST_CASE("Support for save save frames") {
    SECTION("basic file") {
        auto parser = cifxx::parser(std::ifstream(DATADIR "save.cif"));
        auto blocks = parser.parse();

        REQUIRE(blocks.size() == 1);
        CHECK(blocks[0].name() == "sav");
        CHECK(get(blocks[0], "_tag1").as_number() == 2.0);

        CHECK(blocks[0].save().size() == 1);
        auto it = blocks[0].save().find("test");
        REQUIRE(it != blocks[0].save().end());

        auto& save = it->second;
        CHECK(get(save, "_tag2").as_string() == "Hey");
        CHECK(get(save, "_tag3").as_number() == 45.245);
        CHECK(get(save, "_looped").as_vector().size() == 4);
    }

    SECTION("Actual CIF dictionary: PDBX mmCIF v5.0") {
        std::ifstream file(DATADIR "mmcif_pdbx_v50.dic");
        auto blocks = parser(file).parse();
        REQUIRE(blocks.size() == 1);
        auto block = blocks[0];

        CHECK(block.name() == "mmcif_pdbx.dic");
        CHECK(block.save().size() == 6725);

        auto it = block.save().find("pdbx_nmr_sample_details");
        CHECK(it != block.save().end());

        // Weird case with CIF formated data inside a string ...
        auto& data = it->second;
        auto category_examples = get(data, "_category_examples.case").as_vector();
        CHECK(category_examples.size() == 1);
        auto expected = R"(
 loop_
 _pdbx_nmr_sample_details.solution_id
 _pdbx_nmr_sample_details.solvent_system
 _pdbx_nmr_sample_details.contents
 1 MCP-1 '2 mM U-15N,13C, H2O 90 %, D2O 10 %'
 2 MCP-1 '1 mM U-50% 15N, MCP-1 1 mM U-50% 13C, H2O 90 %, D2O 10 %'
 3 MCP-1 '2 mM U-15N, H2O 90 %, D2O 10 %'
)";
        CHECK(category_examples[0].as_string() == expected);
    }
}
