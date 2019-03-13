# Cifxx, a C++11 header only CIF files parser

[![Build Status](https://travis-ci.org/chemfiles/cifxx.svg?branch=master)](https://travis-ci.org/chemfiles/cifxx)

Cifxx is a CIF files parser written in modern C++ (C++11), in an single header,
distributed under the 3 clauses BSD license.

It parses files that follow the CIF/STAR grammar, as described in
https://www.iucr.org/resources/cif/spec/version1.1/cifsyntax. This file format
is used by the [RSCB PDB archive](https://www.rcsb.org/) for description of
protein. It originates from and is also used by the crystallographic community,
where is is used to store atomic coordinates with symmetry in a given space
group.

Cifxx does not perform any validation of any sort, or coordinate generation from
the symmetry group: this is left to the application.

# Features

- No dependencies outside of the standard library
- Single header, easy to use
- Parses all valid CIF files (except for bugs. Please report any you find!)
- Liberal license, usable in commercial applications

# Installation

Grap the [header](dist/cifxx.hpp), and drop it in your project. You're good to
go!

# Usage

The main class is the `parser` class, used to read files:

```cpp
auto file = std::ifstream("file.cif");
auto parser = cifxx::parser(file);
```

Any kind of iterator on `char` can be passed to the constructor, a `std::string`
formatted as CIF data also works.

Parsing the file can throw `cifxx::error`, and return a `std::vector` of `data`
blocks:

```cpp
try {
    auto blocks = parser.parse();
} catch (const cifxx::error& e) {
    // invalid file
}
```

Each data block have a name, and a set of tag => values associations

```cpp
auto block = blocks[0];

// Block name
auto name = block.name();

// Find a given tag in the data block
auto it = block.find("_tag");
if (it != block.end()) {
    auto value = *it;
}

// Iterate over all tags in the data block
for (auto& it: block) {
    if (it.first == "_tag") {
        auto value = it.second;
    }
}
```

Values can have multiple types:

```cpp
// Missing data
if value.is_missing() {

}

// string data, possibly multiple line string
if value.is_string() {
    auto string = value.as_string();
}

// numeric data
if value.is_number() {
    auto number = value.as_number();
}

// vector data, created by a loop_ construct
if value.is_vector() {
    auto vector = value.as_vector();
    for (auto& value: vector) {
        // each vector element is a cifxx::value,
        // which can again have various type.
    }
}
```

# License

Guillaume Fraux created and maintains Cifxx, which is distributed under the
[3 clauses BSD license](LICENSE). By contributing to Cifxx, you agree to
distribute your contributions under the same license.
