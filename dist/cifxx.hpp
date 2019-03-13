#line 1 "cifxx.hpp"
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

#ifndef CIFXX_HPP
#define CIFXX_HPP
#line 1 "cifxx/types.hpp"
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

#ifndef CIFXX_TYPES_HPP
#define CIFXX_TYPES_HPP

#include <string>
#include <vector>
#include <stdexcept>

namespace cifxx {

class value;

/// String type used for string values
using string_t = std::string;
/// Floating point type used for numeric values
using number_t = double;
/// Vector type used for vector values
using vector_t = std::vector<value>;

/// Exception class for all errors
class error: public std::runtime_error {
public:
    explicit error(std::string message): std::runtime_error(std::move(message)) {}
};

}

#endif
#line 1 "cifxx/token.hpp"
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

#ifndef CIFXX_TOKEN_HPP
#define CIFXX_TOKEN_HPP

#include <cassert>


namespace cifxx {

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
#line 1 "cifxx/parser.hpp"
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

#ifndef CIFXX_PARSER_HPP
#define CIFXX_PARSER_HPP

#include <streambuf>
#include <istream>
#line 1 "data.hpp"
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
#line 1 "value.hpp"
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

#ifndef CIFXX_VALUE_HPP
#define CIFXX_VALUE_HPP



namespace cifxx {

/// Possible values in CIF data block.
///
/// A `cifxx::value` can be a floating point number, a string or a vector of
/// `cifxx::value`. It is represented as a tagged union.
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
        /// A vector of `cifxx::value`
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
#line 33 "data.hpp"
namespace cifxx {

/// A data block in a CIF file
class data final {
public:
    using iterator = std::map<std::string, value>::iterator;
    using const_iterator = std::map<std::string, value>::const_iterator;

    /// Create a new data block with the given `name`
    data(std::string name): name_(std::move(name)), data_() {}
    data(const data&) = default;
    data(data&&) = default;
    data& operator=(const data&) = default;
    data& operator=(data&&) = default;

    /// Get the name of this data block
    const std::string& name() const {
        return name_;
    }

    /// Lookup the given `key` in this block.
    ///
    /// @returns an iterator pointing to the key/value pair if the key is
    ///          present in the data, or `data::end()` if the key is not present.
    const_iterator find(const std::string& key) const {
        return data_.find(key);
    }

    /// Find and return the given `key` in this block.
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

    /// Insert a value in the data block, associated with the given `tag`.
    ///
    /// If there is already a value associated with this key, no insertion is
    /// performed, and the return value is `(iterator, false)` where `iterator`
    /// is pointing to the data entry that prevented insertion. If the insertion
    /// took place, the return value is `(iterator, true)` where `iterator`
    /// is pointing to the newly inserted data.
    std::pair<iterator, bool> insert(std::string tag, value val) {
        if (!is_tag_name(tag)) {
            throw error(tag + " is not a valid data tag name");
        }
        return data_.emplace(std::move(tag), std::move(val));
    }

    /// Get the first entry of this data block
    const_iterator begin() const {
        return data_.begin();
    }

    /// Get the end of this data block
    const_iterator end() const {
        return data_.end();
    }

private:
    std::string name_;
    std::map<std::string, value> data_;
};

}

#endif
#line 1 "tokenizer.hpp"
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

#ifndef CIFXX_TOKENIZER_HPP
#define CIFXX_TOKENIZER_HPP

#include <algorithm>


namespace cifxx {

/// Check if a given char is an ordinary char
inline bool is_ordinary_char(char c) {
    return c > 32 && c < 127 && c != '"' && c != '#' && c != '$' &&
           c != '\'' && c != ';' && c != '_' && c != '[' && c != ']';
}

/// Check if a given char is end of line
inline bool is_eol(char c) {
    return c == '\r' || c == '\n';
}

/// Check if a given char is a whitespace
inline bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || is_eol(c);
}

/// Check if a given char is a given digit
inline bool is_digit(char c) {
    return '0' <= c && c <= '9';
}

/// An helper class for the tokenizer, checking if a given char matches a
/// pattern
class char_checker {
public:
    /// Create a `char_checker` checking if the char is equal to the given `value`
    /*implicit*/ char_checker(char value): value_(value) {}

    /// Create a `char_checker` using the given `function`
    /*implicit*/ char_checker(bool (*function)(char)): function_(function) {}

    /// Check a single char
    bool operator()(char c) {
        if (function_) {
            return function_(c);
        } else {
            return c == value_;
        }
    }
private:
    char value_ = '\0';
    bool (*function_)(char) = nullptr;
};


class tokenizer final {
public:
    explicit tokenizer(std::string input):
        input_(std::move(input)), current_(input_.begin()), end_(input_.end()) {}

    /// Yield the next token
    token next() {
        skip_comment_and_whitespace();
        if (finished()) {
            return token::eof();
        } else if (check('\'')) {
            return string();
        } else if (check('\"')) {
            return string();
        } else if (check(';') && previous_is_eol()) {
            advance();
            return multilines_string();
        } else {
            std::string content;
            // unquoted text or other values
            content.push_back(advance());
            while (!finished()) {
                if (check(is_non_blank_char)) {
                    content.push_back(advance());
                    continue;
                } else {
                    // we found a whitespace
                    break;
                }
            }

            // check for reserved words, we only need to do this with
            // unquoted strings
            auto begining = content.substr(0, 5);
            std::transform(begining.begin(), begining.end(), begining.begin(), ::tolower);
            if (begining == "data_") {
                return token::data(content.substr(5));
            } else if (begining == "save_") {
                return token::save(content.substr(5));
            } else if (begining == "loop_") {
                return token::loop();
            } else if (begining == "stop_") {
                return token::stop();
            } else if (begining == "globa") {
                begining = content;
                std::transform(begining.begin(), begining.end(), begining.begin(), ::tolower);
                if (begining == "global_") {
                    return token::global();
                } else {
                    return token_for_value(std::move(content));
                }
            } else {
                return token_for_value(std::move(content));
            }
        }
    }

private:

    /// Check if we reached the end of the input
    bool finished() const {
        return current_ == end_;
    }

    /// Advance the current char by one and return the current char. If the input
    /// stream is finished, return '\0'
    char advance() {
        if (!finished()) {
            return *current_++;
        } else {
            return '\0';
        }
    }

    /// Check if the current char matches the given checker
    bool check(char_checker checker) const {
        if (finished()) {
            return false;
        }
        return checker(*current_);
    }

    /// Check if the current char matches the given checker, and call `advance`
    /// if so
    bool match(char_checker checker) {
        if (check(checker)) {
            advance();
            return true;
        } else {
            return false;
        }
    }

    /// Check if the previous char is the begining of the stream or end of line
    bool previous_is_eol() const {
        if (current_ == input_.begin()) {
            return true;
        } else {
            return is_eol(current_[-1]);
        }
    }

    /// Check if the next char is the end of the stream or end of line
    bool next_is_eol() const {
        if (finished() || current_ + 1 == end_) {
            return true;
        }
        return is_eol(current_[1]);
    }

    /// Check if the next char is the end of the stream or a whitespace
    bool next_is_whitespace() const {
        if (finished() || current_ + 1 == end_) {
            return true;
        }
        return is_whitespace(current_[1]);
    }

    /// Skip all whitespaes and comments in the char stream
    void skip_comment_and_whitespace() {
        while (!finished()) {
            if (match(is_whitespace)) {
                // skip whitespaces
                continue;
            } else if (match('#')) {
                // skip comment as needed
                while (!finished() && !check('\r') && !check('\n')) {
                    advance();
                    // skip character. We don't check for windows style line
                    // ending (\r\n), because any remaining \n will be skipped
                    // later.
                }
                continue;
            } else {
                return;
            }
        }
    }

    /// Parse a quoted string token
    token string() {
        auto quote = advance();
        assert(quote == '\'' || quote == '"');
        std::string content;
        while (!finished()) {
            if (check(quote) && next_is_whitespace()) {
                advance();
                break;
            } else {
                content.push_back(advance());
            }
        }
        return token::string(std::move(content));
    }

    /// Parse a multi-lines string token
    token multilines_string() {
        std::string content;
        while (!finished()) {
            if (check(';')) {
                if (previous_is_eol() && next_is_eol()) {
                    advance();
                    break;
                }
                advance();
            } else {
                content.push_back(advance());
            }
        }
        return token::string(std::move(content));
    }

    /// Parse a token from the given `content`
    token token_for_value(std::string content) const {
        if (content == "?") {
            return token::question_mark();
        } else if (content == ".") {
            return token::dot();
        }

        if (!content.empty() && content[0] == '_') {
            return token::tag(content);
        }

        std::string number = content;
        // check if we have the `number(precision)` form
        if (content.length() >= 4) {
            auto last = content.length() - 1;
            auto lparen = content.rfind('(');
            if (lparen != std::string::npos && content[last] == ')') {
                number = content.substr(0, lparen) + content.substr(lparen + 1, last - lparen - 1);
            }
        }

        // try to get a real
        try {
            std::size_t processed = 0;
            number_t value = std::stod(number, &processed);
            if (processed == number.length()) {
                return token::number(value);
            }
        } catch (const std::invalid_argument&) {
            // do nothing, this is not a real
        } catch (const std::out_of_range&) {
            throw error("real value " + content + " is too big for 64-bit float type");
        }

        if (content.empty()) {
            throw error("invalid empty unquoted string value");
        } else if (content[0] == '$' || content[0] == '[' || content[0] == ']') {
            throw error(
                "invalid string value '" + content + "': '" + content[0] +
                "' is not allowed as the first character of unquoted strings"
            );
        }

        // default to a string value
        return token::string(content);
    }

    std::string input_;
    string_t::const_iterator current_;
    string_t::const_iterator end_;
};

}

#endif
#line 35 "cifxx/parser.hpp"
namespace cifxx {

class parser final {
public:
    explicit parser(std::string input): tokenizer_(std::move(input)), current_(tokenizer_.next()) {}

    template<typename Stream>
    explicit parser(Stream&& input): parser({
        std::istreambuf_iterator<char>(input),
        std::istreambuf_iterator<char>()
    }) {}

    parser(parser&&) = default;
    parser& operator=(parser&&) = default;

    std::vector<data> parse() {
        std::vector<data> data_blocks;
        while (!finished()) {
            data_blocks.push_back(data_block());
        }
        return data_blocks;
    }

private:
    bool finished() const {
        return current_.kind() == token::Eof;
    }

    /// Advance the current token by one and return the current token.
    token advance() {
        if (!finished()) {
            auto other = tokenizer_.next();
            std::swap(other, current_);
            return other;
        } else {
            return current_;
        }
    }

    /// Check if the current token have a given kind
    bool check(token::Kind kind) const {
        return current_.kind() == kind;
    }

    /// Read a single data block
    data data_block() {
        if (!check(token::Data)) {
            throw error("expected 'data_' at the begining of the data block, got " + current_.print());
        }
        auto block = data(advance().as_string());

        while (!finished()) {
            if (check(token::Data)) {
                break;
            } else if (check(token::Tag)) {
                read_tag(block);
            } else if (check(token::Loop)) {
                read_loop(block);
            } else if (check(token::Save)) {
                throw error("save frame are not implemented");
            } else {
                throw error("expected a tag, a loop or a save frame in data block, got " + current_.print());
            }
        }

        return block;
    }

    /// Read a single tag + value
    void read_tag(data& block) {
        auto tag_name = advance().as_tag();

        if (check(token::Dot) || check(token::QuestionMark)) {
            advance();
            block.insert(tag_name, value::missing());
        } else if (check(token::Number)) {
            block.insert(tag_name, advance().as_number());
        } else if (check(token::String)) {
            block.insert(tag_name, advance().as_string());
        } else {
            throw error("expected a value for tag " + tag_name + " , got " + current_.print());
        }
    }

    /// Read a loop section
    void read_loop(data& block) {
        auto loop = advance();
        assert(loop.kind() == token::Loop);

        std::vector<std::pair<std::string, vector_t>> values;
        while (check(token::Tag)) {
            values.push_back({advance().as_tag(), vector_t()});
        }

        size_t current = 0;
        while (!finished()) {
            size_t index = current % values.size();
            if (check(token::Dot) || check(token::QuestionMark)) {
                advance();
                values[index].second.push_back(value::missing());
            } else if (check(token::Number)) {
                values[index].second.push_back(advance().as_number());
            } else if (check(token::String)) {
                values[index].second.push_back(advance().as_string());
            } else {
                break;
            }
            current++;
        }

        if (current % values.size() != 0) {
            throw error(
                "not enough values in the last loop iteration: expected " +
                std::to_string(values.size()) + " got " +
                std::to_string(current % values.size())
            );
        }

        for (auto it: values) {
            block.insert(std::move(it.first), std::move(it.second));
        }
    }

    tokenizer tokenizer_;
    token current_;
};

}

#endif
#line 37 "cifxx.hpp"
#endif
