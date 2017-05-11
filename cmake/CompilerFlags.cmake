include(CheckCXXCompilerFlag)

set(CMAKE_REQUIRED_QUIET YES)

add_definitions("-UNDEBUG")

# C++11 support.
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXXOX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    if(MSVC)
        if(MSVC_VERSION LESS 1900)
            message(SEND_ERROR "MSVC < 14.0 is not supported. Please update your compiler or use mingw")
        endif()
    else()
        message(SEND_ERROR "The ${CMAKE_CXX_COMPILER} compiler lacks C++11 support. Use another compiler.")
    endif()
endif()

macro(add_warning_flag _flag_)
    CHECK_CXX_COMPILER_FLAG("${_flag_}" CXX_SUPPORTS${_flag_})
    if(CXX_SUPPORTS${_flag_})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_flag_}")
    endif()
endmacro()

set(PACIF_CXX_WARNINGS "")

macro(remove_msvc_warning _warn_)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd${_warn_}")
endmacro()

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/Wall" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    endif()

    if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/Wall" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall")
    endif()

    # Disable other warnings
else()
    # Add some warnings in debug mode
    # Basic set of warnings
    add_warning_flag("-Wall")
    add_warning_flag("-Wextra")
    # Initialization and convertion values
    add_warning_flag("-Wuninitialized")
    add_warning_flag("-Wconversion")
    add_warning_flag("-Wsign-conversion")
    add_warning_flag("-Wsign-promo")
    # C++11 functionalities
    add_warning_flag("-Wsuggest-override")
    add_warning_flag("-Wsuggest-final-types")
    # C++ standard conformance
    add_warning_flag("-Wpedantic")
    add_warning_flag("-pedantic")
    # The compiler is your friend
    add_warning_flag("-Wdocumentation")
    add_warning_flag("-Wdeprecated")
    add_warning_flag("-Wextra-semi")
    add_warning_flag("-Wnon-virtual-dtor")
    add_warning_flag("-Wold-style-cast")
    add_warning_flag("-Wcast-align")
    add_warning_flag("-Wunused")
    add_warning_flag("-Woverloaded-virtual")
    add_warning_flag("-Wundefined-func-template")
    add_warning_flag("-Wmissing-prototypes")
    add_warning_flag("-Wmissing-variable-declarations")

    # Not everyone is as smart as clang for code reachability
    add_warning_flag("-Wno-covered-switch-default")
    add_warning_flag("-Wno-unreachable-code-break")
endif()
