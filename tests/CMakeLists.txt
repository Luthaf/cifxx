add_library(catch STATIC catch/catch.cpp)

function(cifxx_test _file_)
    get_filename_component(_name_ ${_file_} NAME_WE)
    add_executable(${_name_} ${_file_})
    target_link_libraries(${_name_} catch cifxx)
    add_test(NAME ${_name_}
        COMMAND ${_name_}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()

file(GLOB all_test_files
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

foreach(test_file IN LISTS all_test_files)
    cifxx_test(${test_file})
endforeach(test_file)

target_compile_definitions(parser PRIVATE "-DDATADIR=\"${CMAKE_CURRENT_SOURCE_DIR}/data/\"")

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data/mmcif_pdbx_v50.dic")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xf mmcif_pdbx_v50.dic.tgz
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data
    )
endif()
