// Copyright (c) 2017, Guillaume Fraux
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

#ifndef PACIF_TOKEN_HPP
#define PACIF_TOKEN_HPP

#include <cassert>
#include "types.hpp"

namespace pacif {

/// Basic token in CIF grammar
class token final {
public:
    enum kind {
        Eof,        // end of file (end of input)
        Loop,       // `loop_` literal
        Stop,       // `stop_` literal
        Global,     // `global_` literal
        Integer,    // an integer value
        Real,       // a floating point value
        String,     // a string value
        Data,       // data frame header
        Save,       // save frame header
    };

    /// Create a new token representing the end of file
    static token eof() {
        return token(Eof);
    }

    /// Create a new token representing the `loop_` literal
    static token loop() {
        return token(Loop);
    }

    /// Create a new token representing the `save_` literal
    static token stop() {
        return token(Stop);
    }

    /// Create a new token representing the `global_` literal
    static token global() {
        return token(Global);
    }

    /// Create a new token representing an integer value
    static token integer(integer_t value) {
        return token(value);
    }

    /// Create a new token representing a real value
    static token real(real_t value) {
        return token(value);
    }

    /// Create a new token representing a data frame header with this `name`
    static token data(string_t name) {
        return token(Data, name);
    }

    /// Create a new token representing a save frame header with this `name`
    static token save(string_t name) {
        return token(Save, name);
    }

    /// Create a new token representing a string `value`
    static token string(string_t value) {
        return token(String, value);
    }

    /// The copy constructor for tokens
    token(const token& other): token(Eof) {
        *this = other;
    }

    /// The copy assignement operator for tokens
    token& operator=(const token& other) {
        this->~token();
        this->kind_ = other.kind_;
        switch (this->kind_) {
        case String:
        case Save:
        case Data:
            new (&this->string_) string_t(other.string_);
            break;
        case Real:
            new (&this->real_) real_t(other.real_);
            break;
        case Integer:
            new (&this->integer_) integer_t(other.integer_);
            break;
        case Eof:
        case Loop:
        case Stop:
        case Global:
            break; // nothing to do
        }
        return *this;
    }

    /// The move constructor for tokens
    token(token&& other): token(Eof) {
        *this = std::move(other);
    }

    /// The move assignement operator for tokens
    token& operator=(token&& other) {
        this->~token();
        this->kind_ = other.kind_;
        switch (this->kind_) {
        case String:
        case Save:
        case Data:
            new (&this->string_) string_t(std::move(other.string_));
            break;
        case Real:
            new (&this->real_) real_t(std::move(other.real_));
            break;
        case Integer:
            new (&this->integer_) integer_t(std::move(other.integer_));
            break;
        case Eof:
        case Loop:
        case Stop:
        case Global:
            break; // nothing to do
        }
        return *this;
    }

    /// The destructor for tokens
    ~token() {
        switch (this->kind_) {
        case String:
        case Save:
        case Data:
            this->string_.~string_t();
            break;
        case Eof:
        case Loop:
        case Stop:
        case Global:
        case Integer:
        case Real:
            break; // nothing to do
        }
    }

    /// Get the token kind
    kind get_kind() const {
        return kind_;
    }

    /// Get the string data in this token, if the token has the `String`,
    /// `Data` or `Tag` kind.
    const string_t& as_string() const {
        if (kind_ == String || kind_ == Data || kind_ == Save) {
            return string_;
        } else {
            throw error("tried to access string data on a non-string token");
        }
    }

    /// Get the integer data in this token, if the token has the `Integer`
    /// kind.
    integer_t as_integer() const {
        if (kind_ == Integer) {
            return integer_;
        } else {
            throw error("tried to access integer data on a non-integer token");
        }
    }

    /// Get the floating point data in this token, if the token has the
    /// `Real` kind.
    real_t as_real() const {
        if (kind_ == Real) {
            return real_;
        } else {
            throw error("tried to access real data on a non-real token");
        }
    }

private:
    /// Contructor to be used for tokens without data attached
    explicit token(kind kind): kind_(kind) {
        assert(kind_ != Real);
        assert(kind_ != Integer);
        assert(kind_ != String);
        assert(kind_ != Data);
        assert(kind_ != Save);
    }

    /// Constructor for tokens with string data attached
    token(kind kind, string_t string): kind_(kind), string_(std::move(string)) {
        assert(kind == String || kind == Data || kind == Save);
    }

    /// Constructor for `Real` tokens
    explicit token(real_t real): kind_(Real), real_(real) {}

    /// Constructor for `Integer` tokens
    explicit token(integer_t integer): kind_(Integer), integer_(integer) {}

    kind kind_;
    union {
        integer_t integer_;
        string_t string_;
        real_t real_;
    };
};

}

#endif
