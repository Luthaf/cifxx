// Copyright (c) 2017-2018, Guillaume Fraux
// All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
// SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

#ifndef CIFXX_DATA_HPP
#define CIFXX_DATA_HPP

#include <map>

#include "value.hpp"
#include "token.hpp"

namespace cifxx {

/// Basic data storage for both data blocks and save blocks
class basic_data {
public:
    using iterator = std::map<std::string, value>::const_iterator;

    basic_data() = default;
    basic_data(const basic_data&) = default;
    basic_data(basic_data&&) = default;
    basic_data& operator=(const basic_data&) = default;
    basic_data& operator=(basic_data&&) = default;

    /// Lookup the given `key` in this data set.
    ///
    /// @returns an iterator pointing to the key/value pair if the key is
    ///          present in the data, or `data::end()` if the key is not present.
    iterator find(const std::string& key) const {
        return data_.find(key);
    }

    /// Find and return the given `key` in this data set.
    ///
    /// @throws cifxx::error if their is no value associated with `key`.
    /// @returns the value associated with `key`.
    const value& get(const std::string& key) const {
        auto it = data_.find(key);
        if (it == data_.end()) {
            throw cifxx::error("could not find " + key + " in this CIF data block");
        }
        return it->second;
    }

    /// Insert a value in the data set, associated with the given `tag`.
    ///
    /// If there is already a value associated with this key, no insertion is
    /// performed, and the return value is `(iterator, false)` where `iterator`
    /// is pointing to the data entry that prevented insertion. If the insertion
    /// took place, the return value is `(iterator, true)` where `iterator`
    /// is pointing to the newly inserted data.
    std::pair<iterator, bool> emplace(std::string tag, value val) {
        if (!is_tag_name(tag)) {
            throw error(tag + " is not a valid data tag name");
        }
        return data_.emplace(std::move(tag), std::move(val));
    }

    /// Get the first entry of this data set
    iterator begin() const {
        return data_.begin();
    }

    /// Get the end of this data set
    iterator end() const {
        return data_.end();
    }

    /// Get the number of items in this data set
    size_t size() const {
        return data_.size();
    }

    /// Check if this data set is empty
    bool empty() const {
        return data_.empty();
    }

private:
    std::map<std::string, value> data_;
};

/// A data block in a CIF file
class data final: public basic_data {
public:
    /// Create a new data block with the given `name`
    data(std::string name): name_(std::move(name)) {}
    data(const data&) = default;
    data(data&&) = default;
    data& operator=(const data&) = default;
    data& operator=(data&&) = default;

    /// Get the name of this data block
    const std::string& name() const {
        return name_;
    }

    /// Add a `save` block with the given `name` in this data block
    void add_save(std::string name, basic_data save) {
        save_.emplace(std::move(name), std::move(save));
    }

    /// Get the save frames in this data block
    const std::map<std::string, basic_data>& save() const {
        return save_;
    }

private:
    std::string name_;
    std::map<std::string, basic_data> save_;
};

}

#endif
