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

#ifndef PACIF_PARSER_HPP
#define PACIF_PARSER_HPP

#include <algorithm>
#include "token.hpp"

namespace pacif {

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
                        return token_for_value(std::move(content));
                    } else {
                        content.push_back(*current_);
                    }
                }
        } else if (*current_ == '"') {  // double quoted string
                while (++current_ != end_) {
                    if (*current_ == '"' && (current_ + 1 == end_ || is_whitespace(current_[1]))) {
                        return token_for_value(std::move(content));
                    } else {
                        content.push_back(*current_);
                    }
                }
            } else if (*current_ == ';' && previous_was_eol()) {  // multilines string
            while (++current_ != end_) {
                if (*current_ == ';' && previous_was_eol()) {
                    return token_for_value(std::move(content));
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
                    return token_for_value(std::move(content));
                }
            }
            return token_for_value(std::move(content));
        }
        throw error("should be unreachable");
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

#endif
