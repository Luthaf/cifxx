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

#ifndef PACIF_TOKEN_HPP
#define PACIF_TOKEN_HPP

#include <cassert>
#include "types.hpp"

namespace pacif {

/// Check if a given char is a non whitespace printable char
inline bool is_non_blank_char(char c) {
    return c > 32 && c < 127;
}

/// Check if `name` is a CIF tag name.
///
///     <tag_name> = '_' {<non_blank_char>}+
inline bool is_tag_name(const std::string& name) {
    if (name.length() < 2) return false;
    if (name[0] != '_') return false;
    auto begin = name.begin();
    auto result = is_non_blank_char(*(begin++));
    while (begin != name.end()) {
        result = result && is_non_blank_char(*begin);
        begin++;
    }
    return result;
}

/// Basic token in CIF grammar
class token final {
public:
    enum Kind {
        Eof,            // end of file (end of input)
        Loop,           // `loop_` literal
        Stop,           // `stop_` literal
        Global,         // `global_` literal
        Tag,            // a tag
        Number,         // a numeric value
        String,         // a string value
        Data,           // data frame header
        Save,           // save frame header
        QuestionMark,   // `?` literal
        Dot,            // `.` literal
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

    /// Create a new token representing the `?` literal
    static token question_mark() {
        return token(QuestionMark);
    }

    /// Create a new token representing the `.` literal
    static token dot() {
        return token(Dot);
    }

    /// Create a new token representing a number value
    static token number(number_t value) {
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

    /// Create a new token representing a tag with the given `name`
    static token tag(string_t name) {
        assert(is_tag_name(name));
        return token(Tag, name);
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
        case Tag:
            new (&this->string_) string_t(other.string_);
            break;
        case Number:
            new (&this->number_) number_t(other.number_);
            break;
        case Eof:
        case Loop:
        case Stop:
        case Global:
        case Dot:
        case QuestionMark:
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
        case Tag:
            new (&this->string_) string_t(std::move(other.string_));
            break;
        case Number:
            new (&this->number_) number_t(std::move(other.number_));
            break;
        case Eof:
        case Loop:
        case Stop:
        case Global:
        case Dot:
        case QuestionMark:
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
        case Tag:
            this->string_.~string_t();
            break;
        case Eof:
        case Loop:
        case Stop:
        case Global:
        case Number:
        case Dot:
        case QuestionMark:
            break; // nothing to do
        }
    }

    /// Get the token kind
    Kind kind() const {
        return kind_;
    }

    /// Get the string data in this token, if the token has the `String`,
    /// `Data`, `Save` or `Tag` kind.
    const string_t& as_string() const {
        if (kind_ == String || kind_ == Data || kind_ == Save || kind_ == Tag) {
            return string_;
        } else {
            throw error("tried to access string data on a non-string token " + print());
        }
    }

    /// Get the tag name of this token, if the token has the `Tag` kind.
    const string_t& as_tag() const {
        if (kind_ == Tag) {
            return string_;
        } else {
            throw error("tried to access tag data on a non-tag token " + print());
        }
    }

    /// Get the numeric in this token, if the token has the `Number` kind.
    number_t as_number() const {
        if (kind_ == Number) {
            return number_;
        } else {
            throw error("tried to access real data on a non-real token " + print());
        }
    }

    std::string print() const {
        switch (this->kind_) {
        case String:
        case Tag:
            return this->as_string();
        case Save:
            return "save_" + this->as_string();
        case Data:
            return "data_" + this->as_string();
        case Eof:
            return "<eof>";
        case Loop:
            return "loop_";
        case Stop:
            return "stop_";
        case Global:
            return "global_";
        case Number:
            return std::to_string(number_);
        case Dot:
            return ".";
        case QuestionMark:
            return "?";
        }
        throw error("unreachable");
    }

private:
    /// Contructor to be used for tokens without data attached
    explicit token(Kind kind): kind_(kind) {
        assert(kind_ != Number);
        assert(kind_ != String);
        assert(kind_ != Data);
        assert(kind_ != Save);
    }

    /// Constructor for tokens with string data attached
    token(Kind kind, string_t string): kind_(kind), string_(std::move(string)) {
        assert(kind == String || kind == Data || kind == Save || kind == Tag);
    }

    /// Constructor for `Real` tokens
    explicit token(number_t number): kind_(Number), number_(number) {}

    Kind kind_;
    union {
        string_t string_;
        number_t number_;
    };
};

}

#endif
