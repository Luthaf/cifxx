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

#include <cstdio>
#include <algorithm>
#include "token.hpp"

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
            content.reserve(128);
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
                if (content.size() == 5) {
                    return token::save_end();
                } else {
                    return token::save(content.substr(5));
                }
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

    /// Get the current line number in the input, starting at 1
    size_t line() const {
        return line_;
    }

private:

    /// Check if we reached the end of the input
    bool finished() const {
        return current_ == end_;
    }

    /// Advance the current char by one and return the current char. If the
    /// input stream is finished, return '\0'
    char advance() {
        if (!finished()) {
            auto c = *current_++;

            // increment line counter
            if (current_[-1] == '\n') {
                line_++;
            } else if (current_[-1] == '\r') {
                if (*current_ == '\n') {
                    // windows style line, don't increment line count, it will
                    // be done on the next call to advance
                } else {
                    // classic MacOS style
                    line_++;
                }
            }

            return c;
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
        content.reserve(1024);
        while (!finished()) {
            if (check(';')) {
                if (previous_is_eol()) {
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
        number_t value = 0;
        int processed = 0;
        auto assigned = std::sscanf(number.c_str(), "%lf%n", &value, &processed);
        if (assigned == 1 && number.size() == static_cast<size_t>(processed)) {
            return token::number(value);
        }

        if (content.empty()) {
            throw_error("invalid empty unquoted string value");
        } else if (content[0] == '$' || content[0] == '[' || content[0] == ']') {
            throw_error(
                "invalid string value '" + content + "': '" + content[0] +
                "' is not allowed as the first character of unquoted strings"
            );
        }

        // default to a string value
        return token::string(std::move(content));
    }

    [[noreturn]] void throw_error(std::string message) const {
        throw error(
            "error on line " + std::to_string(line_) + ": " + message
        );
    }

    std::string input_;
    size_t line_ = 1;

    string_t::const_iterator current_;
    string_t::const_iterator end_;
};

}

#endif
