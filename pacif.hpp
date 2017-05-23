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
#include <exception>

namespace pacif {

class value;
using string_t = std::string;
using real_t = double;
using vec_t = std::vector<value>;

struct error: public std::runtime_error {
    explicit error(std::string message): std::runtime_error(std::move(message)) {}
    error(const error&) = default;
    error(error&&) = default;
    error& operator=(const error&) = default;
    error& operator=(error&&) = default;
    virtual ~error() = default;
};

class value final {
public:
    enum class kind {
        missing,
        string,
        real,
        vec,
    };

    value(): kind_(kind::missing) {}
    explicit value(string_t string): kind_(kind::string), string_(std::move(string)) {}
    explicit value(real_t real): kind_(kind::real), real_(real) {}
    explicit value(vec_t vec): kind_(kind::vec), vec_(std::move(vec)) {}

    value(const value& other): value() {
        *this = other;
    }

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

    value(value&& other): value() {
        *this = std::move(other);
    }

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

    bool is_missing() const {
        return this->kind_ == kind::missing;
    }

    bool is_string() const {
        return this->kind_ == kind::string;
    }

    bool is_vec() const {
        return this->kind_ == kind::vec;
    }

    bool is_real() const {
        return this->kind_ == kind::real;
    }

    kind get_kind() const {
        return this->kind_;
    }

    const string_t& as_string() const {
        if (this->kind_ == kind::string) {
            return this->string_;
        } else {
            throw error("This is not a string value");
        }
    }

    real_t as_real() const {
        if (this->kind_ == kind::real) {
            return this->real_;
        } else {
            throw error("This is not a real value");
        }
    }

    const vec_t& as_vec() const {
        if (this->kind_ == kind::vec) {
            return this->vec_;
        } else {
            throw error("This is not a vec value");
        }
    }

private:
    kind kind_;
    union {
        string_t string_;
        real_t real_;
        vec_t vec_;
    };
};

}

#endif
