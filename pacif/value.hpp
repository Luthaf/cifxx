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

#ifndef PACIF_VALUE_HPP
#define PACIF_VALUE_HPP

#include "types.hpp"

namespace pacif {

/// Possible values in CIF data block.
///
/// A `pacif::value` can be a floating point number, a string or a vector of
/// `pacif::value`. It is represented as a tagged union.
class value final {
public:
    /// Available kinds of value
    enum Kind {
        /// A missing value
        // TODO: should missing and invalid values be different?
        Missing,
        /// A numeric value, represented as a floating point number
        Number,
        /// A string value
        String,
        /// A vector of `pacif::value`
        Vector,
    };

    static value missing() {
        return value();
    }

    /// Create a string value containing `string`
    /*implicit*/ value(string_t string): kind_(Kind::String), string_(std::move(string)) {}
    /*implicit*/ value(char* string): kind_(Kind::String), string_(std::move(string)) {}
    /*implicit*/ value(const char* string): kind_(Kind::String), string_(std::move(string)) {}

    /// Create a real value containing `real`
    /*implicit*/ value(number_t number): kind_(Kind::Number), number_(number) {}

    /// Create a vector value containing `vec`
    /*implicit*/ value(vector_t vector): kind_(Kind::Vector), vector_(std::move(vector)) {}

    /// The copy constructor for values
    value(const value& other): value() {
        *this = other;
    }

    /// The copy assignement operator for values
    value& operator=(const value& other) {
        this->~value();
        this->kind_ = other.kind_;
        switch (this->kind_) {
        case Kind::Missing:
            break; // nothing to do
        case Kind::String:
            new (&this->string_) string_t(other.string_);
            break;
        case Kind::Vector:
            new (&this->vector_) vector_t(other.vector_);
            break;
        case Kind::Number:
            new (&this->number_) number_t(other.number_);
            break;
        }
        return *this;
    }

    /// The move constructor for values
    value(value&& other): value() {
        *this = std::move(other);
    }

    /// The move assignement operator for values
    value& operator=(value&& other) {
        this->~value();
        this->kind_ = other.kind_;
        switch (this->kind_) {
        case Kind::Missing:
            break; // nothing to do
        case Kind::String:
            new (&this->string_) string_t(std::move(other.string_));
            break;
        case Kind::Vector:
            new (&this->vector_) vector_t(std::move(other.vector_));
            break;
        case Kind::Number:
            new (&this->number_) number_t(std::move(other.number_));
            break;
        }
        return *this;
    }

    /// The destructor for values
    ~value() {
        switch (this->kind_) {
        case Kind::String:
            this->string_.~string_t();
            break;
        case Kind::Vector:
            this->vector_.~vector_t();
            break;
        case Kind::Number:
            break; // nothing to do
        case Kind::Missing:
            break; // nothing to do
        }
    }

    /// Check if this value is a missing value
    bool is_missing() const {
        return this->kind_ == Kind::Missing;
    }

    /// Check if this value is a string
    bool is_string() const {
        return this->kind_ == Kind::String;
    }

    /// Check if this value is a vector
    bool is_vector() const {
        return this->kind_ == Kind::Vector;
    }

    /// Check if this value is a number
    bool is_number() const {
        return this->kind_ == Kind::Number;
    }

    /// Get the kind of this value
    Kind kind() const {
        return this->kind_;
    }

    /// Get this value as a string
    ///
    /// @throw if the value is not a string
    const string_t& as_string() const {
        if (this->kind_ == Kind::String) {
            return this->string_;
        } else {
            throw error("called value::as_string, but this is not a string value");
        }
    }

    /// Get this value as a number
    ///
    /// @throw if the value is not a number
    number_t as_number() const {
        if (this->kind_ == Kind::Number) {
            return this->number_;
        } else {
            throw error("called value::as_number, but this is not a number value");
        }
    }

    /// Get this value as a vector
    ///
    /// @throw if the value is not a vector
    const vector_t& as_vector() const {
        if (this->kind_ == Kind::Vector) {
            return this->vector_;
        } else {
            throw error("called value::as_vector, but this is not a vector value");
        }
    }

private:
    /// Create a missing value
    value(): kind_(Kind::Missing) {}

    /// Kind of the stored value
    Kind kind_;
    /// Value data storage, as an union
    union {
        number_t number_;
        string_t string_;
        vector_t vector_;
    };
};

}

#endif
