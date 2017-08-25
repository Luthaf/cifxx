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

#ifndef PACIF_HPP
#define PACIF_HPP

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <cassert>
#include <algorithm>
#include <exception>

namespace pacif {

class value;
/// String type used for string values
using string_t = std::string;
/// Floating point type used for real values
using real_t = double;
/// Integer type used for integer values
using integer_t = int64_t;
/// Vector type used for vector values
using vec_t = std::vector<value>;

/// Exception class for all errors
struct error: public std::runtime_error {
    explicit error(std::string message): std::runtime_error(std::move(message)) {}
};

/// Collection of functions to check conformance to the CIF grammar
namespace parsing {
    /// Basic token in CIF grammar
    class token final {
    public:
        enum kind {
            Eof,        // end of file (end of input)
            Loop,       // loop_ literal
            Stop,       // stop_ literal
            Global,     // global_ literal
            Integer,    // an integer value
            Real,       // a floating point value
            String,     // a string value
            Data,       // data frame header
            Save,       // save frame header
        };

        /// Create a new token representing the end of file
        static token eof() {return token(Eof);}
        /// Create a new token representing the `loop_` literal
        static token loop() {return token(Loop);}
        /// Create a new token representing the `save_` literal
        static token stop() {return token(Stop);}
        /// Create a new token representing the `global_` literal
        static token global() {return token(Global);}
        /// Create a new token representing an integer value
        static token integer(integer_t value) {return token(value);}
        /// Create a new token representing a real value
        static token real(real_t value) {return token(value);}
        /// Create a new token representing a data frame header with this `name`
        static token data(string_t name) {return token(Data, name);}
        /// Create a new token representing a save frame header with this `name`
        static token save(string_t name) {return token(Save, name);}
        /// Create a new token representing a string `value`
        static token string(string_t value) {return token(String, value);}

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
        explicit token(kind kind, string_t string): kind_(kind), string_(std::move(string)) {
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

    /// Check if a given char is an ordinary char
    inline bool is_ordinary_char(char c) {
        return c > 32 && c < 127 && c != '"' && c != '#' && c != '$' &&
               c != '\'' && c != ';' && c != '_' && c != '[' && c != ']';
    }

    /// Check if a given char is a non whitespace printable char
    inline bool is_non_blank_char(char c) {
        return c > 32 && c < 127;
    }

    /// Check if a given char is a non whitespace printable char
    inline bool is_whitespace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n';
    }

    /// Check if a given char is a given digit
    inline bool is_digit(char c) {
        return '0' <= c && c <= '9';
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

    class parser final {
    public:
        explicit parser(std::string input):
            input_(std::move(input)), current_(input_.begin()), end_(input_.end()) {}

        /// Yield the next token
        token next() {
            bool skipped_whitespace_and_comments = false;
            while (!skipped_whitespace_and_comments) {
                if (current_ == end_) { // did we reached the end of input?
                    return token::eof();
                } else if (is_whitespace(*current_)) { // skip whitespaces
                    current_++;
                    continue;
                } else if (*current_ == '#') {  // skip comment as needed
                    while (++current_ != end_ && *current_ != '\n' && *current_ != '\r') {
                        // skip character. We don't check for windows style line
                        // ending (\r\n), because any remaining \n will be skipped
                        // later.
                    }
                    continue;
                } else {
                    skipped_whitespace_and_comments = true;
                }
            }

            // Get the token content
            std::string content;
            if (*current_ == '\'') {  // single quoted string
                while (++current_ != end_) {
                    if (*current_ == '\'' && (current_ + 1 == end_ || is_whitespace(current_[1]))) {
                        return token_for_value(content);
                    } else {
                        content.push_back(*current_);
                    }
                }
            } else if (*current_ == '"') {  // double quoted string
                while (++current_ != end_) {
                    if (*current_ == '"' && (current_ + 1 == end_ || is_whitespace(current_[1]))) {
                        return token_for_value(content);
                    } else {
                        content.push_back(*current_);
                    }
                }
            } else if (*current_ == ';' && previous_was_eol()) {  // multilines string
                while (++current_ != end_) {
                    if (*current_ == ';' && previous_was_eol()) {
                        return token_for_value(content);
                    } else {
                        content.push_back(*current_);
                    }
                }
            } else {
                // unquoted text or other values
                content.push_back(*current_);
                while (++current_ != end_) {
                    if (is_non_blank_char(*current_)) {
                        content.push_back(*current_);
                    } else {
                        // we found a whitespace
                        break;
                    }
                }

                // check for reserved words, we only need to do this with
                // unquoted strings
                auto beggining = content.substr(0, 5);
                std::transform(beggining.begin(), beggining.end(), beggining.begin(), ::tolower);
                if (beggining == "data_") {
                    return token::data(content.substr(5));
                } else if (beggining == "save_") {
                    return token::save(content.substr(5));
                } else if (beggining == "loop_") {
                    return token::loop();
                } else if (beggining == "stop_") {
                    return token::stop();
                } else if (beggining == "globa") {
                    beggining = content;
                    std::transform(beggining.begin(), beggining.end(), beggining.begin(), ::tolower);
                    if (beggining == "global_") {
                        return token::global();
                    } else {
                        return token::string(content);
                    }
                }
                return token_for_value(content);
            }
            throw error("this should be unreachable");
        }

    private:
        bool previous_was_eol() const {
            assert(current_ > input_.begin());
            auto c = current_[-1];
            return c != '\n' || c != '\r';
        }

        token token_for_value(std::string content) const {
            std::string number = content;
            // check if we have the `number(precision)` form
            if (content.length() >= 4) {
                auto last = content.length() - 1;
                if (content[last] == ')' && is_digit(content[last - 1]) && content[last - 2] == '(') {
                    number = content.substr(0, last - 2) + content[last - 1];
                }
            }

            // try to get an integer
            try {
                std::size_t processed = 0;
                integer_t value = std::stoll(number, &processed);
                if (processed == number.length()) {
                    return token::integer(value);
                }
            } catch (const std::invalid_argument&) {
                // do nothing, this is not an integer
            } catch (const std::out_of_range&) {
                throw error("integer value " + content + " is too big for 64-bit integer type");
            }

            // try to get a real
            try {
                std::size_t processed = 0;
                real_t value = std::stod(number, &processed);
                if (processed == number.length()) {
                    return token::real(value);
                }
            } catch (const std::invalid_argument&) {
                // do nothing, this is not a real
            } catch (const std::out_of_range&) {
                throw error("real value " + content + " is too big for 64-bit float type");
            }

            // default to a string value
            return token::string(content);
        }

        const std::string input_;
        string_t::const_iterator current_;
        const string_t::const_iterator end_;
    };
}

/// Possible values in CIF data block.
///
/// A `pacif::value` can be a floating point number, a string or a vector of
/// `pacif::value`. It is represented as a tagged union.
class value final {
public:
    /// Available kinds of value
    enum class kind {
        /// A missing value
        missing,
        /// A string value
        string,
        /// A real value, represented as a floating point number
        real,
        /// A vector of `pacif::value`
        vec,
    };

    /// Create a missing value
    value(): kind_(kind::missing) {}
    /// Create a string value containing `string`
    explicit value(string_t string): kind_(kind::string), string_(std::move(string)) {}
    /// Create a real value containing `real`
    explicit value(real_t real): kind_(kind::real), real_(real) {}
    /// Create a vector value containing `vec`
    explicit value(vec_t vec): kind_(kind::vec), vec_(std::move(vec)) {}

    /// The copy constructor for values
    value(const value& other): value() {
        *this = other;
    }

    /// The copy assignement operator for values
    value& operator=(const value& other) {
        this->~value();
        this->kind_ = other.kind_;
        switch (this->kind_) {
        case kind::missing:
            break; // nothing to do
        case kind::string:
            new (&this->string_) string_t(other.string_);
            break;
        case kind::vec:
            new (&this->vec_) vec_t(other.vec_);
            break;
        case kind::real:
            new (&this->real_) real_t(other.real_);
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
        case kind::missing:
            break; // nothing to do
        case kind::string:
            new (&this->string_) string_t(std::move(other.string_));
            break;
        case kind::vec:
            new (&this->vec_) vec_t(std::move(other.vec_));
            break;
        case kind::real:
            new (&this->real_) real_t(std::move(other.real_));
            break;
        }
        return *this;
    }

    /// The destructor for values
    ~value() {
        switch (this->kind_) {
        case kind::string:
            this->string_.~string_t();
            break;
        case kind::vec:
            this->vec_.~vec_t();
            break;
        case kind::real:
            break; // nothing to do
        case kind::missing:
            break; // nothing to do
        }
    }

    /// Check if this value is a missing value
    bool is_missing() const {
        return this->kind_ == kind::missing;
    }

    /// Check if this value is a string
    bool is_string() const {
        return this->kind_ == kind::string;
    }

    /// Check if this value is a vector
    bool is_vec() const {
        return this->kind_ == kind::vec;
    }

    /// Check if this value is a real
    bool is_real() const {
        return this->kind_ == kind::real;
    }

    /// Get the kind of this value
    kind get_kind() const {
        return this->kind_;
    }

    /// Get this value as a string
    ///
    /// @throw if the value is not a string
    const string_t& as_string() const {
        if (this->kind_ == kind::string) {
            return this->string_;
        } else {
            throw error("called value::as_string, but this is not a string value");
        }
    }

    /// Get this value as a real
    ///
    /// @throw if the value is not a real
    real_t as_real() const {
        if (this->kind_ == kind::real) {
            return this->real_;
        } else {
            throw error("called value::as_real, but this is not a real value");
        }
    }

    /// Get this value as a vector
    ///
    /// @throw if the value is not a vector
    const vec_t& as_vec() const {
        if (this->kind_ == kind::vec) {
            return this->vec_;
        } else {
            throw error("called value::as_vec, but this is not a vector value");
        }
    }

private:
    /// Kind of the stored value
    kind kind_;
    /// Value data storage, as an union
    union {
        string_t string_;
        real_t real_;
        vec_t vec_;
    };
};

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
    /// @returns an iterator pointing to the value associated with `key` if the
    ///          key is present, or `data::end()` if the key is not present.
    const_iterator find(const std::string& key) const {
        return data_.find(key);
    }

    /// Insert a data point of type `T` and value `val` in the data block,
    /// associated with the given `key`. `T` must be convertible to
    /// `pacif::value`.
    ///
    /// If there is already a value associated with this key, no insertion is
    /// performed, and the return value is `(iterator, false)` where `iterator`
    /// is pointing to the data entry that prevented insertion. If the insertion
    /// took place, the return value is `(iterator, true)` where `iterator`
    /// is pointing to the newly inserted data.
    template<class T>
    std::pair<iterator, bool> insert(std::string key, T val) {
        if (!parsing::is_tag_name(key)) {
            throw error(key + " is not a valid data tag name");
        }
        return data_.emplace(std::move(key), value(std::move(val)));
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
