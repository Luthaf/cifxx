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
#line 1 "external/string_view.hpp"
// Copyright 2017-2019 by Martin Moene
//
// string-view lite, a C++17-like string_view for C++98 and later.
// For more information see https://github.com/martinmoene/string-view-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)



#ifndef NONSTD_SV_LITE_H_INCLUDED
#define NONSTD_SV_LITE_H_INCLUDED

#define string_view_lite_MAJOR  1
#define string_view_lite_MINOR  3
#define string_view_lite_PATCH  0

#define string_view_lite_VERSION  nssv_STRINGIFY(string_view_lite_MAJOR) "." nssv_STRINGIFY(string_view_lite_MINOR) "." nssv_STRINGIFY(string_view_lite_PATCH)

#define nssv_STRINGIFY(  x )  nssv_STRINGIFY_( x )
#define nssv_STRINGIFY_( x )  #x

// string-view lite configuration:

#define nssv_STRING_VIEW_DEFAULT  0
#define nssv_STRING_VIEW_NONSTD   1
#define nssv_STRING_VIEW_STD      2

#if !defined(nssv_CONFIG_SELECT_STRING_VIEW)
# define nssv_CONFIG_SELECT_STRING_VIEW  ( nssv_HAVE_STD_STRING_VIEW ? nssv_STRING_VIEW_STD : nssv_STRING_VIEW_NONSTD )
#endif

#if defined(nssv_CONFIG_SELECT_STD_STRING_VIEW) || defined(nssv_CONFIG_SELECT_NONSTD_STRING_VIEW)
# error nssv_CONFIG_SELECT_STD_STRING_VIEW and nssv_CONFIG_SELECT_NONSTD_STRING_VIEW are deprecated and removed, please use nssv_CONFIG_SELECT_STRING_VIEW=nssv_STRING_VIEW_...
#endif

#ifndef  nssv_CONFIG_STD_SV_OPERATOR
# define nssv_CONFIG_STD_SV_OPERATOR  0
#endif

#ifndef  nssv_CONFIG_USR_SV_OPERATOR
# define nssv_CONFIG_USR_SV_OPERATOR  1
#endif

#ifdef   nssv_CONFIG_CONVERSION_STD_STRING
# define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS   nssv_CONFIG_CONVERSION_STD_STRING
# define nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS  nssv_CONFIG_CONVERSION_STD_STRING
#endif

#ifndef  nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS
# define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS  1
#endif

#ifndef  nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
# define nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS  1
#endif

// Control presence of exception handling (try and auto discover):

#ifndef nssv_CONFIG_NO_EXCEPTIONS
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#  define nssv_CONFIG_NO_EXCEPTIONS  0
# else
#  define nssv_CONFIG_NO_EXCEPTIONS  1
# endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   nssv_CPLUSPLUS
# if defined(_MSVC_LANG) && !defined(__clang__)
#  define nssv_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define nssv_CPLUSPLUS  __cplusplus
# endif
#endif

#define nssv_CPP98_OR_GREATER  ( nssv_CPLUSPLUS >= 199711L )
#define nssv_CPP11_OR_GREATER  ( nssv_CPLUSPLUS >= 201103L )
#define nssv_CPP11_OR_GREATER_ ( nssv_CPLUSPLUS >= 201103L )
#define nssv_CPP14_OR_GREATER  ( nssv_CPLUSPLUS >= 201402L )
#define nssv_CPP17_OR_GREATER  ( nssv_CPLUSPLUS >= 201703L )
#define nssv_CPP20_OR_GREATER  ( nssv_CPLUSPLUS >= 202000L )

// use C++17 std::string_view if available and requested:

#if ( nssv_CPLUSPLUS >= 201703 ) && defined(__has_include)
# if __has_include( <string_view> )
#  define nssv_HAVE_STD_STRING_VIEW  1
# else
#  define nssv_HAVE_STD_STRING_VIEW  0
# endif
#else
# define  nssv_HAVE_STD_STRING_VIEW  0
#endif

#define  nssv_USES_STD_STRING_VIEW  ( (nssv_CONFIG_SELECT_STRING_VIEW == nssv_STRING_VIEW_STD) || ((nssv_CONFIG_SELECT_STRING_VIEW == nssv_STRING_VIEW_DEFAULT) && nssv_HAVE_STD_STRING_VIEW) )

#define nssv_HAVE_STARTS_WITH ( nssv_CPP20_OR_GREATER || !nssv_USES_STD_STRING_VIEW )
#define nssv_HAVE_ENDS_WITH     nssv_HAVE_STARTS_WITH

//
// Use C++17 std::string_view:
//

#if ( (nssv_CONFIG_SELECT_STRING_VIEW == 2) || ((nssv_CONFIG_SELECT_STRING_VIEW == 0) && nssv_HAVE_STD_STRING_VIEW) )

#include <string_view>

// Extensions for std::string:

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {

template< class CharT, class Traits, class Allocator = std::allocator<CharT> >
std::basic_string<CharT, Traits, Allocator>
to_string( std::basic_string_view<CharT, Traits> v, Allocator const & a = Allocator() )
{
    return std::basic_string<CharT,Traits, Allocator>( v.begin(), v.end(), a );
}

template< class CharT, class Traits, class Allocator >
std::basic_string_view<CharT, Traits>
to_string_view( std::basic_string<CharT, Traits, Allocator> const & s )
{
    return std::basic_string_view<CharT, Traits>( s.data(), s.size() );
}

// Literal operators sv and _sv:

#if nssv_CONFIG_STD_SV_OPERATOR

using namespace std::literals::string_view_literals;

#endif

#if nssv_CONFIG_USR_SV_OPERATOR

inline namespace literals {
inline namespace string_view_literals {


constexpr std::string_view operator "" _sv( const char* str, size_t len ) noexcept  // (1)
{
    return std::string_view{ str, len };
}

constexpr std::u16string_view operator "" _sv( const char16_t* str, size_t len ) noexcept  // (2)
{
    return std::u16string_view{ str, len };
}

constexpr std::u32string_view operator "" _sv( const char32_t* str, size_t len ) noexcept  // (3)
{
    return std::u32string_view{ str, len };
}

constexpr std::wstring_view operator "" _sv( const wchar_t* str, size_t len ) noexcept  // (4)
{
    return std::wstring_view{ str, len };
}

}} // namespace literals::string_view_literals

#endif // nssv_CONFIG_USR_SV_OPERATOR

} // namespace nonstd

#endif // nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {

using std::string_view;
using std::wstring_view;
using std::u16string_view;
using std::u32string_view;
using std::basic_string_view;

// literal "sv" and "_sv", see above

using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;

using std::operator<<;

} // namespace nonstd

#else // nssv_HAVE_STD_STRING_VIEW

//
// Before C++17: use string_view lite:
//

// Compiler versions:
//
// MSVC++ 6.0  _MSC_VER == 1200 (Visual Studio 6.0)
// MSVC++ 7.0  _MSC_VER == 1300 (Visual Studio .NET 2002)
// MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
// MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)

#if defined(_MSC_VER) && !defined(__clang__)
# define nssv_COMPILER_MSVC_VER      (_MSC_VER )
# define nssv_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define nssv_COMPILER_MSVC_VER      0
# define nssv_COMPILER_MSVC_VERSION  0
#endif

#define nssv_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined(__clang__)
# define nssv_COMPILER_CLANG_VERSION  nssv_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define nssv_COMPILER_CLANG_VERSION    0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define nssv_COMPILER_GNUC_VERSION  nssv_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define nssv_COMPILER_GNUC_VERSION    0
#endif

// half-open range [lo..hi):
#define nssv_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Presence of language and library features:

#ifdef _HAS_CPP0X
# define nssv_HAS_CPP0X  _HAS_CPP0X
#else
# define nssv_HAS_CPP0X  0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for variant-lite:

#if nssv_COMPILER_MSVC_VER >= 1900
# undef  nssv_CPP11_OR_GREATER
# define nssv_CPP11_OR_GREATER  1
#endif

#define nssv_CPP11_90   (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1500)
#define nssv_CPP11_100  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1600)
#define nssv_CPP11_110  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1700)
#define nssv_CPP11_120  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1800)
#define nssv_CPP11_140  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1900)
#define nssv_CPP11_141  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1910)

#define nssv_CPP14_000  (nssv_CPP14_OR_GREATER)
#define nssv_CPP17_000  (nssv_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define nssv_HAVE_CONSTEXPR_11          nssv_CPP11_140
#define nssv_HAVE_EXPLICIT_CONVERSION   nssv_CPP11_140
#define nssv_HAVE_INLINE_NAMESPACE      nssv_CPP11_140
#define nssv_HAVE_NOEXCEPT              nssv_CPP11_140
#define nssv_HAVE_NULLPTR               nssv_CPP11_100
#define nssv_HAVE_REF_QUALIFIER         nssv_CPP11_140
#define nssv_HAVE_UNICODE_LITERALS      nssv_CPP11_140
#define nssv_HAVE_USER_DEFINED_LITERALS nssv_CPP11_140
#define nssv_HAVE_WCHAR16_T             nssv_CPP11_100
#define nssv_HAVE_WCHAR32_T             nssv_CPP11_100

#if ! ( ( ( nssv_CPLUSPLUS >= 201103 ) && nssv_COMPILER_CLANG_VERSION ) || nssv_BETWEEN( nssv_COMPILER_CLANG_VERSION, 300, 400 ) )
# define nssv_HAVE_STD_DEFINED_LITERALS  nssv_CPP11_140
#else
# define nssv_HAVE_STD_DEFINED_LITERALS  0
#endif

// Presence of C++14 language features:

#define nssv_HAVE_CONSTEXPR_14          nssv_CPP14_000

// Presence of C++17 language features:

#define nssv_HAVE_NODISCARD             nssv_CPP17_000

// Presence of C++ library features:

#define nssv_HAVE_STD_HASH              nssv_CPP11_120

// C++ feature usage:

#if nssv_HAVE_CONSTEXPR_11
# define nssv_constexpr  constexpr
#else
# define nssv_constexpr  /*constexpr*/
#endif

#if nssv_HAVE_CONSTEXPR_14
# define nssv_constexpr14  constexpr
#else
# define nssv_constexpr14  /*constexpr*/
#endif

#if nssv_HAVE_EXPLICIT_CONVERSION
# define nssv_explicit  explicit
#else
# define nssv_explicit  /*explicit*/
#endif

#if nssv_HAVE_INLINE_NAMESPACE
# define nssv_inline_ns  inline
#else
# define nssv_inline_ns  /*inline*/
#endif

#if nssv_HAVE_NOEXCEPT
# define nssv_noexcept  noexcept
#else
# define nssv_noexcept  /*noexcept*/
#endif

//#if nssv_HAVE_REF_QUALIFIER
//# define nssv_ref_qual  &
//# define nssv_refref_qual  &&
//#else
//# define nssv_ref_qual  /*&*/
//# define nssv_refref_qual  /*&&*/
//#endif

#if nssv_HAVE_NULLPTR
# define nssv_nullptr  nullptr
#else
# define nssv_nullptr  NULL
#endif

#if nssv_HAVE_NODISCARD
# define nssv_nodiscard  [[nodiscard]]
#else
# define nssv_nodiscard  /*[[nodiscard]]*/
#endif

// Additional includes:

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <ostream>
#include <string>   // std::char_traits<>

#if ! nssv_CONFIG_NO_EXCEPTIONS
# include <stdexcept>
#endif

#if ( nssv_CPLUSPLUS >= 201103 )
# include <type_traits>
#endif

// Clang, GNUC, MSVC warning suppression macros:

#if defined(__clang__)
# pragma clang diagnostic ignored "-Wreserved-user-defined-literal"
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wuser-defined-literals"
#elif defined(__GNUC__)
# pragma  GCC  diagnostic push
# pragma  GCC  diagnostic ignored "-Wliteral-suffix"
#endif // __clang__

#if nssv_COMPILER_MSVC_VERSION >= 140
# define nssv_SUPPRESS_MSGSL_WARNING(expr)        [[gsl::suppress(expr)]]
# define nssv_SUPPRESS_MSVC_WARNING(code, descr)  __pragma(warning(suppress: code) )
# define nssv_DISABLE_MSVC_WARNINGS(codes)        __pragma(warning(push))  __pragma(warning(disable: codes))
#else
# define nssv_SUPPRESS_MSGSL_WARNING(expr)
# define nssv_SUPPRESS_MSVC_WARNING(code, descr)
# define nssv_DISABLE_MSVC_WARNINGS(codes)
#endif

#if defined(__clang__)
# define nssv_RESTORE_WARNINGS()  _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
# define nssv_RESTORE_WARNINGS()  _Pragma("GCC diagnostic pop")
#elif nssv_COMPILER_MSVC_VERSION >= 140
# define nssv_RESTORE_WARNINGS()  __pragma(warning(pop ))
#else
# define nssv_RESTORE_WARNINGS()
#endif

// Suppress the following MSVC (GSL) warnings:
// - C4455, non-gsl   : 'operator ""sv': literal suffix identifiers that do not
//                      start with an underscore are reserved
// - C26472, gsl::t.1 : don't use a static_cast for arithmetic conversions;
//                      use brace initialization, gsl::narrow_cast or gsl::narow
// - C26481: gsl::b.1 : don't use pointer arithmetic. Use span instead

nssv_DISABLE_MSVC_WARNINGS( 4455 26481 26472 )
//nssv_DISABLE_CLANG_WARNINGS( "-Wuser-defined-literals" )
//nssv_DISABLE_GNUC_WARNINGS( -Wliteral-suffix )

namespace nonstd { namespace sv_lite {

#if ( nssv_CPLUSPLUS >= 201103 )

namespace detail {

// Expect tail call optimization to make length() non-recursive:

template< typename CharT >
inline constexpr std::size_t length( CharT * s, std::size_t result = 0 )
{
    return *s == '\0' ? result : length( s + 1, result + 1 );
}

} // namespace detail

#endif // nssv_CPP11_OR_GREATER

template
<
    class CharT,
    class Traits = std::char_traits<CharT>
>
class basic_string_view;

//
// basic_string_view:
//

template
<
    class CharT,
    class Traits /* = std::char_traits<CharT> */
>
class basic_string_view
{
public:
    // Member types:

    typedef Traits traits_type;
    typedef CharT  value_type;

    typedef CharT       * pointer;
    typedef CharT const * const_pointer;
    typedef CharT       & reference;
    typedef CharT const & const_reference;

    typedef const_pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator< const_iterator > reverse_iterator;
    typedef	std::reverse_iterator< const_iterator > const_reverse_iterator;

    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;

    // 24.4.2.1 Construction and assignment:

    nssv_constexpr basic_string_view() nssv_noexcept
        : data_( nssv_nullptr )
        , size_( 0 )
    {}

#if ( nssv_CPLUSPLUS >= 201103 )
    nssv_constexpr basic_string_view( basic_string_view const & other ) nssv_noexcept = default;
#else
    nssv_constexpr basic_string_view( basic_string_view const & other ) nssv_noexcept
        : data_( other.data_)
        , size_( other.size_)
    {}
#endif

    nssv_constexpr basic_string_view( CharT const * s, size_type count ) nssv_noexcept // non-standard noexcept
        : data_( s )
        , size_( count )
    {}

    nssv_constexpr basic_string_view( CharT const * s) nssv_noexcept // non-standard noexcept
        : data_( s )
#if ( nssv_CPLUSPLUS >= 201703 )
        , size_( Traits::length(s) )
#elif ( nssv_CPLUSPLUS >= 201103 )
        , size_( detail::length(s) )
#else
        , size_( Traits::length(s) )
#endif
    {}

    // Assignment:

#if ( nssv_CPLUSPLUS >= 201103 )
    nssv_constexpr14 basic_string_view & operator=( basic_string_view const & other ) nssv_noexcept = default;
#else
    nssv_constexpr14 basic_string_view & operator=( basic_string_view const & other ) nssv_noexcept
    {
        data_ = other.data_;
        size_ = other.size_;
        return *this;
    }
#endif

    // 24.4.2.2 Iterator support:

    nssv_constexpr const_iterator begin()  const nssv_noexcept { return data_;         }
    nssv_constexpr const_iterator end()    const nssv_noexcept { return data_ + size_; }

    nssv_constexpr const_iterator cbegin() const nssv_noexcept { return begin(); }
    nssv_constexpr const_iterator cend()   const nssv_noexcept { return end();   }

    nssv_constexpr const_reverse_iterator rbegin()  const nssv_noexcept { return const_reverse_iterator( end() );   }
    nssv_constexpr const_reverse_iterator rend()    const nssv_noexcept { return const_reverse_iterator( begin() ); }

    nssv_constexpr const_reverse_iterator crbegin() const nssv_noexcept { return rbegin(); }
    nssv_constexpr const_reverse_iterator crend()   const nssv_noexcept { return rend();   }

    // 24.4.2.3 Capacity:

    nssv_constexpr size_type size()     const nssv_noexcept { return size_; }
    nssv_constexpr size_type length()   const nssv_noexcept { return size_; }
    nssv_constexpr size_type max_size() const nssv_noexcept { return (std::numeric_limits< size_type >::max)(); }

    // since C++20
    nssv_nodiscard nssv_constexpr bool empty() const nssv_noexcept
    {
        return 0 == size_;
    }

    // 24.4.2.4 Element access:

    nssv_constexpr const_reference operator[]( size_type pos ) const
    {
        return data_at( pos );
    }

    nssv_constexpr14 const_reference at( size_type pos ) const
    {
#if nssv_CONFIG_NO_EXCEPTIONS
        assert( pos < size() );
#else
        if ( pos >= size() )
        {
            throw std::out_of_range("nonstd::string_view::at()");
        }
#endif
        return data_at( pos );
    }

    nssv_constexpr const_reference front() const { return data_at( 0 );          }
    nssv_constexpr const_reference back()  const { return data_at( size() - 1 ); }

    nssv_constexpr const_pointer   data()  const nssv_noexcept { return data_; }

    // 24.4.2.5 Modifiers:

    nssv_constexpr14 void remove_prefix( size_type n )
    {
        assert( n <= size() );
        data_ += n;
        size_ -= n;
    }

    nssv_constexpr14 void remove_suffix( size_type n )
    {
        assert( n <= size() );
        size_ -= n;
    }

    nssv_constexpr14 void swap( basic_string_view & other ) nssv_noexcept
    {
        using std::swap;
        swap( data_, other.data_ );
        swap( size_, other.size_ );
    }

    // 24.4.2.6 String operations:

    size_type copy( CharT * dest, size_type n, size_type pos = 0 ) const
    {
#if nssv_CONFIG_NO_EXCEPTIONS
        assert( pos <= size() );
#else
        if ( pos > size() )
        {
            throw std::out_of_range("nonstd::string_view::copy()");
        }
#endif
        const size_type rlen = (std::min)( n, size() - pos );

        (void) Traits::copy( dest, data() + pos, rlen );

        return rlen;
    }

    nssv_constexpr14 basic_string_view substr( size_type pos = 0, size_type n = npos ) const
    {
#if nssv_CONFIG_NO_EXCEPTIONS
        assert( pos <= size() );
#else
        if ( pos > size() )
        {
            throw std::out_of_range("nonstd::string_view::substr()");
        }
#endif
        return basic_string_view( data() + pos, (std::min)( n, size() - pos ) );
    }

    // compare(), 6x:

    nssv_constexpr14 int compare( basic_string_view other ) const nssv_noexcept // (1)
    {
        if ( const int result = Traits::compare( data(), other.data(), (std::min)( size(), other.size() ) ) )
        {
            return result;
        }

        return size() == other.size() ? 0 : size() < other.size() ? -1 : 1;
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, basic_string_view other ) const // (2)
    {
        return substr( pos1, n1 ).compare( other );
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, basic_string_view other, size_type pos2, size_type n2 ) const // (3)
    {
        return substr( pos1, n1 ).compare( other.substr( pos2, n2 ) );
    }

    nssv_constexpr int compare( CharT const * s ) const // (4)
    {
        return compare( basic_string_view( s ) );
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, CharT const * s ) const // (5)
    {
        return substr( pos1, n1 ).compare( basic_string_view( s ) );
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, CharT const * s, size_type n2 ) const // (6)
    {
        return substr( pos1, n1 ).compare( basic_string_view( s, n2 ) );
    }

    // 24.4.2.7 Searching:

    // starts_with(), 3x, since C++20:

    nssv_constexpr bool starts_with( basic_string_view v ) const nssv_noexcept  // (1)
    {
        return size() >= v.size() && compare( 0, v.size(), v ) == 0;
    }

    nssv_constexpr bool starts_with( CharT c ) const nssv_noexcept  // (2)
    {
        return starts_with( basic_string_view( &c, 1 ) );
    }

    nssv_constexpr bool starts_with( CharT const * s ) const  // (3)
    {
        return starts_with( basic_string_view( s ) );
    }

    // ends_with(), 3x, since C++20:

    nssv_constexpr bool ends_with( basic_string_view v ) const nssv_noexcept  // (1)
    {
        return size() >= v.size() && compare( size() - v.size(), npos, v ) == 0;
    }

    nssv_constexpr bool ends_with( CharT c ) const nssv_noexcept  // (2)
    {
        return ends_with( basic_string_view( &c, 1 ) );
    }

    nssv_constexpr bool ends_with( CharT const * s ) const  // (3)
    {
        return ends_with( basic_string_view( s ) );
    }

    // find(), 4x:

    nssv_constexpr14 size_type find( basic_string_view v, size_type pos = 0 ) const nssv_noexcept  // (1)
    {
        return assert( v.size() == 0 || v.data() != nssv_nullptr )
            , pos >= size()
            ? npos
            : to_pos( std::search( cbegin() + pos, cend(), v.cbegin(), v.cend(), Traits::eq ) );
    }

    nssv_constexpr14 size_type find( CharT c, size_type pos = 0 ) const nssv_noexcept  // (2)
    {
        return find( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr14 size_type find( CharT const * s, size_type pos, size_type n ) const  // (3)
    {
        return find( basic_string_view( s, n ), pos );
    }

    nssv_constexpr14 size_type find( CharT const * s, size_type pos = 0 ) const  // (4)
    {
        return find( basic_string_view( s ), pos );
    }

    // rfind(), 4x:

    nssv_constexpr14 size_type rfind( basic_string_view v, size_type pos = npos ) const nssv_noexcept  // (1)
    {
        if ( size() < v.size() )
        {
            return npos;
        }

        if ( v.empty() )
        {
            return (std::min)( size(), pos );
        }

        const_iterator last   = cbegin() + (std::min)( size() - v.size(), pos ) + v.size();
        const_iterator result = std::find_end( cbegin(), last, v.cbegin(), v.cend(), Traits::eq );

        return result != last ? size_type( result - cbegin() ) : npos;
    }

    nssv_constexpr14 size_type rfind( CharT c, size_type pos = npos ) const nssv_noexcept  // (2)
    {
        return rfind( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr14 size_type rfind( CharT const * s, size_type pos, size_type n ) const  // (3)
    {
        return rfind( basic_string_view( s, n ), pos );
    }

    nssv_constexpr14 size_type rfind( CharT const * s, size_type pos = npos ) const  // (4)
    {
        return rfind( basic_string_view( s ), pos );
    }

    // find_first_of(), 4x:

    nssv_constexpr size_type find_first_of( basic_string_view v, size_type pos = 0 ) const nssv_noexcept  // (1)
    {
        return pos >= size()
            ? npos
            : to_pos( std::find_first_of( cbegin() + pos, cend(), v.cbegin(), v.cend(), Traits::eq ) );
    }

    nssv_constexpr size_type find_first_of( CharT c, size_type pos = 0 ) const nssv_noexcept  // (2)
    {
        return find_first_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_first_of( CharT const * s, size_type pos, size_type n ) const  // (3)
    {
        return find_first_of( basic_string_view( s, n ), pos );
    }

    nssv_constexpr size_type find_first_of(  CharT const * s, size_type pos = 0 ) const  // (4)
    {
        return find_first_of( basic_string_view( s ), pos );
    }

    // find_last_of(), 4x:

    nssv_constexpr size_type find_last_of( basic_string_view v, size_type pos = npos ) const nssv_noexcept  // (1)
    {
        return empty()
            ? npos
            : pos >= size()
            ? find_last_of( v, size() - 1 )
            : to_pos( std::find_first_of( const_reverse_iterator( cbegin() + pos + 1 ), crend(), v.cbegin(), v.cend(), Traits::eq ) );
    }

    nssv_constexpr size_type find_last_of( CharT c, size_type pos = npos ) const nssv_noexcept  // (2)
    {
        return find_last_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_last_of( CharT const * s, size_type pos, size_type count ) const  // (3)
    {
        return find_last_of( basic_string_view( s, count ), pos );
    }

    nssv_constexpr size_type find_last_of( CharT const * s, size_type pos = npos ) const  // (4)
    {
        return find_last_of( basic_string_view( s ), pos );
    }

    // find_first_not_of(), 4x:

    nssv_constexpr size_type find_first_not_of( basic_string_view v, size_type pos = 0 ) const nssv_noexcept  // (1)
    {
        return pos >= size()
            ? npos
            : to_pos( std::find_if( cbegin() + pos, cend(), not_in_view( v ) ) );
    }

    nssv_constexpr size_type find_first_not_of( CharT c, size_type pos = 0 ) const nssv_noexcept  // (2)
    {
        return find_first_not_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_first_not_of( CharT const * s, size_type pos, size_type count ) const  // (3)
    {
        return find_first_not_of( basic_string_view( s, count ), pos );
    }

    nssv_constexpr size_type find_first_not_of( CharT const * s, size_type pos = 0 ) const  // (4)
    {
        return find_first_not_of( basic_string_view( s ), pos );
    }

    // find_last_not_of(), 4x:

    nssv_constexpr size_type find_last_not_of( basic_string_view v, size_type pos = npos ) const nssv_noexcept  // (1)
    {
        return empty()
            ? npos
            : pos >= size()
            ? find_last_not_of( v, size() - 1 )
            : to_pos( std::find_if( const_reverse_iterator( cbegin() + pos + 1 ), crend(), not_in_view( v ) ) );
    }

    nssv_constexpr size_type find_last_not_of( CharT c, size_type pos = npos ) const nssv_noexcept  // (2)
    {
        return find_last_not_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_last_not_of( CharT const * s, size_type pos, size_type count ) const  // (3)
    {
        return find_last_not_of( basic_string_view( s, count ), pos );
    }

    nssv_constexpr size_type find_last_not_of( CharT const * s, size_type pos = npos ) const  // (4)
    {
        return find_last_not_of( basic_string_view( s ), pos );
    }

    // Constants:

#if ( nssv_CPLUSPLUS >= 201703 )
    static nssv_constexpr size_type npos = size_type(-1);
#elif ( nssv_CPLUSPLUS >= 201103 )
    enum : size_type { npos = size_type(-1) };
#else
    enum { npos = size_type(-1) };
#endif

private:
    struct not_in_view
    {
        const basic_string_view v;

        nssv_constexpr explicit not_in_view( basic_string_view v_ ) : v( v_ ) {}

        nssv_constexpr bool operator()( CharT c ) const
        {
            return npos == v.find_first_of( c );
        }
    };

    nssv_constexpr size_type to_pos( const_iterator it ) const
    {
        return it == cend() ? npos : size_type( it - cbegin() );
    }

    nssv_constexpr size_type to_pos( const_reverse_iterator it ) const
    {
        return it == crend() ? npos : size_type( crend() - it - 1 );
    }

    nssv_constexpr const_reference data_at( size_type pos ) const
    {
#if nssv_BETWEEN( nssv_COMPILER_GNUC_VERSION, 1, 500 )
        return data_[pos];
#else
        return assert( pos < size() ), data_[pos];
#endif
    }

private:
    const_pointer data_;
    size_type     size_;

public:
#if nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS

    template< class Allocator >
    basic_string_view( std::basic_string<CharT, Traits, Allocator> const & s ) nssv_noexcept
        : data_( s.data() )
        , size_( s.size() )
    {}

#if nssv_HAVE_EXPLICIT_CONVERSION

    template< class Allocator >
    explicit operator std::basic_string<CharT, Traits, Allocator>() const
    {
        return to_string( Allocator() );
    }

#endif // nssv_HAVE_EXPLICIT_CONVERSION

#if ( nssv_CPLUSPLUS >= 201103 )

    template< class Allocator = std::allocator<CharT> >
    std::basic_string<CharT, Traits, Allocator>
    to_string( Allocator const & a = Allocator() ) const
    {
        return std::basic_string<CharT, Traits, Allocator>( begin(), end(), a );
    }

#else

    std::basic_string<CharT, Traits>
    to_string() const
    {
        return std::basic_string<CharT, Traits>( begin(), end() );
    }

    template< class Allocator >
    std::basic_string<CharT, Traits, Allocator>
    to_string( Allocator const & a ) const
    {
        return std::basic_string<CharT, Traits, Allocator>( begin(), end(), a );
    }

#endif // nssv_CPP11_OR_GREATER

#endif // nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS
};

//
// Non-member functions:
//

// 24.4.3 Non-member comparison functions:
// lexicographically compare two string views (function template):

template< class CharT, class Traits >
nssv_constexpr bool operator== (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) == 0 ; }

template< class CharT, class Traits >
nssv_constexpr bool operator!= (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) != 0 ; }

template< class CharT, class Traits >
nssv_constexpr bool operator< (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0 ; }

template< class CharT, class Traits >
nssv_constexpr bool operator<= (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0 ; }

template< class CharT, class Traits >
nssv_constexpr bool operator> (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0 ; }

template< class CharT, class Traits >
nssv_constexpr bool operator>= (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0 ; }

// Let S be basic_string_view<CharT, Traits>, and sv be an instance of S.
// Implementations shall provide sufficient additional overloads marked
// constexpr and noexcept so that an object t with an implicit conversion
// to S can be compared according to Table 67.

#if ( nssv_CPLUSPLUS >= 201103 ) && ! nssv_BETWEEN( nssv_COMPILER_MSVC_VERSION, 100, 141 )

#define nssv_BASIC_STRING_VIEW_I(T,U)  typename std::decay< basic_string_view<T,U> >::type

#if nssv_BETWEEN( nssv_COMPILER_MSVC_VERSION, 140, 150 )
# define nssv_MSVC_ORDER(x)  , int=x
#else
# define nssv_MSVC_ORDER(x)  /*, int=x*/
#endif

// ==

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator==(
         basic_string_view  <CharT, Traits> lhs,
    nssv_BASIC_STRING_VIEW_I(CharT, Traits) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) == 0; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator==(
    nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
         basic_string_view  <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

// !=

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator!= (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.size() != rhs.size() || lhs.compare( rhs ) != 0 ; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator!= (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) != 0 ; }

// <

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator< (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0 ; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator< (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0 ; }

// <=

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator<= (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0 ; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator<= (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0 ; }

// >

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator> (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0 ; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator> (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0 ; }

// >=

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator>= (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0 ; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator>= (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0 ; }

#undef nssv_MSVC_ORDER
#undef nssv_BASIC_STRING_VIEW_I

#endif // nssv_CPP11_OR_GREATER

// 24.4.4 Inserters and extractors:

namespace detail {

template< class Stream >
void write_padding( Stream & os, std::streamsize n )
{
    for ( std::streamsize i = 0; i < n; ++i )
        os.rdbuf()->sputc( os.fill() );
}

template< class Stream, class View >
Stream & write_to_stream( Stream & os, View const & sv )
{
    typename Stream::sentry sentry( os );

    if ( !os )
        return os;

    const std::streamsize length = static_cast<std::streamsize>( sv.length() );

    // Whether, and how, to pad:
    const bool      pad = ( length < os.width() );
    const bool left_pad = pad && ( os.flags() & std::ios_base::adjustfield ) == std::ios_base::right;

    if ( left_pad )
        write_padding( os, os.width() - length );

    // Write span characters:
    os.rdbuf()->sputn( sv.begin(), length );

    if ( pad && !left_pad )
        write_padding( os, os.width() - length );

    // Reset output stream width:
    os.width( 0 );

    return os;
}

} // namespace detail

template< class CharT, class Traits >
std::basic_ostream<CharT, Traits> &
operator<<(
    std::basic_ostream<CharT, Traits>& os,
    basic_string_view <CharT, Traits> sv )
{
    return detail::write_to_stream( os, sv );
}

// Several typedefs for common character types are provided:

typedef basic_string_view<char>      string_view;
typedef basic_string_view<wchar_t>   wstring_view;
#if nssv_HAVE_WCHAR16_T
typedef basic_string_view<char16_t>  u16string_view;
typedef basic_string_view<char32_t>  u32string_view;
#endif

}} // namespace nonstd::sv_lite

//
// 24.4.6 Suffix for basic_string_view literals:
//

#if nssv_HAVE_USER_DEFINED_LITERALS

namespace nonstd {
nssv_inline_ns namespace literals {
nssv_inline_ns namespace string_view_literals {

#if nssv_CONFIG_STD_SV_OPERATOR && nssv_HAVE_STD_DEFINED_LITERALS

nssv_constexpr nonstd::sv_lite::string_view operator "" sv( const char* str, size_t len ) nssv_noexcept  // (1)
{
    return nonstd::sv_lite::string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u16string_view operator "" sv( const char16_t* str, size_t len ) nssv_noexcept  // (2)
{
    return nonstd::sv_lite::u16string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u32string_view operator "" sv( const char32_t* str, size_t len ) nssv_noexcept  // (3)
{
    return nonstd::sv_lite::u32string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::wstring_view operator "" sv( const wchar_t* str, size_t len ) nssv_noexcept  // (4)
{
    return nonstd::sv_lite::wstring_view{ str, len };
}

#endif // nssv_CONFIG_STD_SV_OPERATOR && nssv_HAVE_STD_DEFINED_LITERALS

#if nssv_CONFIG_USR_SV_OPERATOR

nssv_constexpr nonstd::sv_lite::string_view operator "" _sv( const char* str, size_t len ) nssv_noexcept  // (1)
{
    return nonstd::sv_lite::string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u16string_view operator "" _sv( const char16_t* str, size_t len ) nssv_noexcept  // (2)
{
    return nonstd::sv_lite::u16string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u32string_view operator "" _sv( const char32_t* str, size_t len ) nssv_noexcept  // (3)
{
    return nonstd::sv_lite::u32string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::wstring_view operator "" _sv( const wchar_t* str, size_t len ) nssv_noexcept  // (4)
{
    return nonstd::sv_lite::wstring_view{ str, len };
}

#endif // nssv_CONFIG_USR_SV_OPERATOR

}}} // namespace nonstd::literals::string_view_literals

#endif

//
// Extensions for std::string:
//

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {
namespace sv_lite {

// Exclude MSVC 14 (19.00): it yields ambiguous to_string():

#if ( nssv_CPLUSPLUS >= 201103 ) && nssv_COMPILER_MSVC_VERSION != 140

template< class CharT, class Traits, class Allocator = std::allocator<CharT> >
std::basic_string<CharT, Traits, Allocator>
to_string( basic_string_view<CharT, Traits> v, Allocator const & a = Allocator() )
{
    return std::basic_string<CharT,Traits, Allocator>( v.begin(), v.end(), a );
}

#else

template< class CharT, class Traits >
std::basic_string<CharT, Traits>
to_string( basic_string_view<CharT, Traits> v )
{
    return std::basic_string<CharT, Traits>( v.begin(), v.end() );
}

template< class CharT, class Traits, class Allocator >
std::basic_string<CharT, Traits, Allocator>
to_string( basic_string_view<CharT, Traits> v, Allocator const & a )
{
    return std::basic_string<CharT, Traits, Allocator>( v.begin(), v.end(), a );
}

#endif // nssv_CPP11_OR_GREATER

template< class CharT, class Traits, class Allocator >
basic_string_view<CharT, Traits>
to_string_view( std::basic_string<CharT, Traits, Allocator> const & s )
{
    return basic_string_view<CharT, Traits>( s.data(), s.size() );
}

}} // namespace nonstd::sv_lite

#endif // nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

//
// make types and algorithms available in namespace nonstd:
//

namespace nonstd {

using sv_lite::basic_string_view;
using sv_lite::string_view;
using sv_lite::wstring_view;

#if nssv_HAVE_WCHAR16_T
using sv_lite::u16string_view;
#endif
#if nssv_HAVE_WCHAR32_T
using sv_lite::u32string_view;
#endif

// literal "sv"

using sv_lite::operator==;
using sv_lite::operator!=;
using sv_lite::operator<;
using sv_lite::operator<=;
using sv_lite::operator>;
using sv_lite::operator>=;

using sv_lite::operator<<;

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
using sv_lite::to_string;
using sv_lite::to_string_view;
#endif

} // namespace nonstd

// 24.4.5 Hash support (C++11):

// Note: The hash value of a string view object is equal to the hash value of
// the corresponding string object.

#if nssv_HAVE_STD_HASH

#include <functional>

namespace std {

template<>
struct hash< nonstd::string_view >
{
public:
    std::size_t operator()( nonstd::string_view v ) const nssv_noexcept
    {
        return std::hash<std::string>()( std::string( v.data(), v.size() ) );
    }
};

template<>
struct hash< nonstd::wstring_view >
{
public:
    std::size_t operator()( nonstd::wstring_view v ) const nssv_noexcept
    {
        return std::hash<std::wstring>()( std::wstring( v.data(), v.size() ) );
    }
};

template<>
struct hash< nonstd::u16string_view >
{
public:
    std::size_t operator()( nonstd::u16string_view v ) const nssv_noexcept
    {
        return std::hash<std::u16string>()( std::u16string( v.data(), v.size() ) );
    }
};

template<>
struct hash< nonstd::u32string_view >
{
public:
    std::size_t operator()( nonstd::u32string_view v ) const nssv_noexcept
    {
        return std::hash<std::u32string>()( std::u32string( v.data(), v.size() ) );
    }
};

} // namespace std

#endif // nssv_HAVE_STD_HASH

nssv_RESTORE_WARNINGS()

#endif // nssv_HAVE_STD_STRING_VIEW
#endif // NONSTD_SV_LITE_H_INCLUDED
#line 35 "cifxx/types.hpp"
namespace cifxx {

class value;

/// String type used for string values
using string_t = std::string;
/// String-view type used in tokenizer/parser
using string_view_t = nonstd::string_view;
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

#include <new>
#include <string>
#include <cassert>



namespace cifxx {

/// Check if a given char is a non whitespace printable char
inline bool is_non_blank_char(char c) {
    return c > 32 && c < 127;
}

/// Check if `name` is a CIF tag name.
///
///     <tag_name> = '_' {<non_blank_char>}+
inline bool is_tag_name(string_view_t name) {
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
        SaveEnd,        // `save_` literal, ends a save block
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

    /// Create a new token representing the `stop_` literal
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
    static token data(string_view_t name) {
        return token(Data, name);
    }
    // overload resolution helper
    static token data(const char* name) {
        return token::data(string_view_t(name));
    }
    // disable calling this funtion on std::string temporaries
    static token data(string_t&& name) = delete;

    /// Create a new token representing a save frame header with this `name`
    static token save(string_view_t name) {
        return token(Save, name);
    }
    // overload resolution helper
    static token save(const char* name) {
        return token::save(string_view_t(name));
    }
    // disable calling this funtion on std::string temporaries
    static token save(string_t&& name) = delete;

    /// Create a new token representing the `save_` literal
    static token save_end() {
        return token(SaveEnd);
    }

    /// Create a new token representing a string `value`
    static token string(string_view_t value) {
        return token(String, std::move(value));
    }
    // overload resolution helper
    static token string(const char* value) {
        return token::string(string_view_t(value));
    }
    // disable calling this funtion on std::string temporaries
    static token string(string_t&& value) = delete;

    /// Create a new token representing a tag with the given `name`
    static token tag(string_view_t name) {
        assert(is_tag_name(name));
        return token(Tag, std::move(name));
    }
    // overload resolution helper
    static token tag(const char* name) {
        return token::tag(string_view_t(name));
    }
    // disable calling this funtion on std::string temporaries
    static token tag(string_t&& name) = delete;

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
            new (&this->string_) string_view_t(other.string_);
            break;
        case Number:
            new (&this->number_) number_t(other.number_);
            break;
        case Dot:
        case Eof:
        case Loop:
        case Stop:
        case Global:
        case SaveEnd:
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
            new (&this->string_) string_view_t(std::move(other.string_));
            break;
        case Number:
            new (&this->number_) number_t(std::move(other.number_));
            break;
        case Dot:
        case Eof:
        case Loop:
        case Stop:
        case Global:
        case SaveEnd:
        case QuestionMark:
            break; // nothing to do
        }
        return *this;
    }

    /// The destructor for tokens
    ~token() {
        // nothing to do
    }

    /// Get the token kind
    Kind kind() const {
        return kind_;
    }

    /// Get the string data in this token, if the token has the `String`,
    /// `Data`, `Save` or `Tag` kind.
    string_view_t as_str_view() const {
        if (kind_ == String || kind_ == Data || kind_ == Save || kind_ == Tag) {
            return string_;
        } else {
            throw error("tried to access string data on a non-string token " + print());
        }
    }

    /// Get the tag name of this token, if the token has the `Tag` kind.
    string_view_t as_tag() const {
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
            return this->as_str_view().to_string();
        case Save:
            return "save_" + this->as_str_view().to_string();
        case SaveEnd:
            return "save_";
        case Data:
            return "data_" + this->as_str_view().to_string();
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
    token(Kind kind, string_view_t string): kind_(kind), string_(string) {
        assert(kind == String || kind == Data || kind == Save || kind == Tag);
    }

    /// Constructor for `Real` tokens
    explicit token(number_t number): kind_(Number), number_(number) {}

    Kind kind_;
    union {
        // Holding a string_view as a data-member is usually not recomended.
        // It is fine here since the corresponding string will be kept alive by
        // the parser.
        string_view_t string_;
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

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <istream>
#include <iterator>
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

#include <new>
#include <string>
#include <utility>



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
#include <string>
#include <utility>





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

#include <cstdio>
#include <cctype>
#include <cassert>

#include <string>
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

/// Check if a given char can start a number
inline bool is_number_start(char c) {
    return is_digit(c) || c == '-' || c == '+' || c == '.';
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

    tokenizer(const tokenizer& other): tokenizer("") {
        *this = other;
    }

    tokenizer(tokenizer&& other): tokenizer("") {
        *this = std::move(other);
    }

    tokenizer& operator=(const tokenizer& other) {
        // compute current position as an offset
        auto offset = other.current_ - other.input_.begin();
        // copy the data
        input_ = other.input_;
        line_ = other.line_;
        // Update the iterators
        current_ = input_.begin() + offset;
        end_ = input_.end();
        return *this;
    }

    tokenizer& operator=(tokenizer&& other) {
        // compute current position as an offset
        auto offset = other.current_ - other.input_.begin();
        // move the data
        input_ = std::move(other.input_);
        line_ = other.line_;
        // Update the iterators
        current_ = input_.begin() + offset;
        end_ = input_.end();
        return *this;
    }

    // disable calling next on rvalues, since the token data will point to
    // deallocated memory
    token next() && = delete;

    /// Yield the next token
    token next() & {
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
            auto start = current_.base();
            size_t count = 0;
            // unquoted text or other values
            while (!finished()) {
                if (check(is_non_blank_char)) {
                    advance();
                    count++;
                } else {
                    // we found a whitespace
                    break;
                }
            }

            // check for reserved words, we only need to do this with
            // unquoted strings
            auto content = string_view_t(start, count);
            auto begining = content.substr(0, 5).to_string();
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
                begining = content.to_string();
                std::transform(begining.begin(), begining.end(), begining.begin(), ::tolower);
                if (begining == "global_") {
                    return token::global();
                } else {
                    return token_for_value(content);
                }
            } else {
                return token_for_value(content);
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
        auto start = current_.base();
        size_t count = 0;
        while (!finished()) {
            if (check(quote) && next_is_whitespace()) {
                advance();
                break;
            } else {
                advance();
                count++;
            }
        }
        return token::string(string_view_t(start, count));
    }

    /// Parse a multi-lines string token
    token multilines_string() {
        auto start = current_.base();
        size_t count = 0;
        while (!finished()) {
            if (check(';')) {
                if (previous_is_eol()) {
                    advance();
                    break;
                }
                advance();
            } else {
                advance();
                count++;
            }
        }
        return token::string(string_view_t(start, count));
    }

    /// Parse a token from the given `content`
    token token_for_value(string_view_t content) const {
        if (content == "?") {
            return token::question_mark();
        } else if (content == ".") {
            return token::dot();
        }

        if (!content.empty() && content[0] == '_') {
            return token::tag(content);
        }

        if (!content.empty() && is_number_start(content[0])) {
            auto number = content.to_string();
            // check if we have the `number(precision)` form
            if (number.length() >= 4) {
                auto last = content.length() - 1;
                auto lparen = content.rfind('(');
                if (lparen != std::string::npos && content[last] == ')') {
                    number = number.substr(0, lparen) + number.substr(lparen + 1, last - lparen - 1);
                }
            }

            number_t value = 0;
            int processed = 0;
            auto assigned = std::sscanf(number.c_str(), "%lf%n", &value, &processed);
            // Only return a number if we could parse one and if it spans the
            // whole string (i.e. not parsing 235 out of "235fgh").
            if (assigned == 1 && number.size() == static_cast<size_t>(processed)) {
                return token::number(value);
            }
        }

        if (content.empty()) {
            throw_error("invalid empty unquoted string value");
        } else if (content[0] == '$' || content[0] == '[' || content[0] == ']') {
            throw_error(
                "invalid string value '" + content.to_string() + "': '" + content[0] +
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

    string_t input_;
    size_t line_ = 1;

    string_t::const_iterator current_;
    string_t::const_iterator end_;
};

}

#endif
#line 43 "cifxx/parser.hpp"
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

    /// Parse a whole file and get all the data blocks inside
    std::vector<data> parse() {
        std::vector<data> data;
        while (!finished()) {
            data.emplace_back(next());
        }
        return data;
    }

    /// Check whether we have read all the data in the file
    bool finished() const {
        return current_.kind() == token::Eof;
    }

    /// Read a single data block from the file
    data next() {
        if (!check(token::Data)) {
            throw_error(
                "expected 'data_' at the begining of the data block, "
                "got '" + current_.print() + "'"
            );
        }
        auto block = data(advance().as_str_view().to_string());

        while (!finished()) {
            if (check(token::Data)) {
                break;
            } else if (check(token::Tag)) {
                read_tag(block);
            } else if (check(token::Loop)) {
                read_loop(block);
            } else if (check(token::Save)) {
                read_save(block);
            } else {
                throw_error(
                    "expected a tag, a loop or a save frame in data block, "
                    "got '" + current_.print() + "'"
                );
            }
        }

        return block;
    }

private:
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

    /// Read a save frame
    void read_save(data& block) {
        auto name = advance().as_str_view();
        auto save = basic_data();

        while (!finished()) {
            if (check(token::SaveEnd)) {
                advance();
                break;
            } else if (check(token::Tag)) {
                read_tag(save);
            } else if (check(token::Loop)) {
                read_loop(save);
            } else if (check(token::Data)) {
                throw_error("expected end of save frame, got a new data block");
            } else if (check(token::Save)) {
                throw_error("expected end of save frame, got a new save frame");
            } else {
                throw_error("expected a tag, a loop or a save frame in data block, got " + current_.print());
            }
        }

        block.add_save(name.to_string(), std::move(save));
    }

    /// Read a single tag + value
    void read_tag(basic_data& data) {
        auto tag_name = advance().as_tag();

        if (check(token::Dot) || check(token::QuestionMark)) {
            advance();
            data.emplace(tag_name.to_string(), value::missing());
        } else if (check(token::Number)) {
            data.emplace(tag_name.to_string(), advance().as_number());
        } else if (check(token::String)) {
            data.emplace(tag_name.to_string(), advance().as_str_view().to_string());
        } else {
            throw_error("expected a value for tag " + tag_name.to_string() + " , got " + current_.print());
        }
    }

    /// Read a loop section
    void read_loop(basic_data& data) {
        auto loop = advance();
        assert(loop.kind() == token::Loop);

        std::vector<std::pair<std::string, vector_t>> values;
        while (check(token::Tag)) {
            values.emplace_back(advance().as_tag().to_string(), vector_t());
        }

        size_t current = 0;
        while (!finished()) {
            size_t index = current % values.size();
            if (check(token::Dot) || check(token::QuestionMark)) {
                advance();
                values[index].second.emplace_back(value::missing());
            } else if (check(token::Number)) {
                values[index].second.emplace_back(advance().as_number());
            } else if (check(token::String)) {
                values[index].second.emplace_back(advance().as_str_view().to_string());
            } else {
                break;
            }
            current++;
        }

        if (current % values.size() != 0) {
            throw_error(
                "not enough values in the last loop iteration: expected " +
                std::to_string(values.size()) + " got " +
                std::to_string(current % values.size())
            );
        }

        for (auto it: values) {
            data.emplace(std::move(it.first), std::move(it.second));
        }
    }

    [[noreturn]] void throw_error(std::string message) {
        throw error(
            "error on line " + std::to_string(tokenizer_.line()) + ": " + message
        );
    }

    tokenizer tokenizer_;
    token current_;
};

}

#endif
#line 37 "cifxx.hpp"
#endif
