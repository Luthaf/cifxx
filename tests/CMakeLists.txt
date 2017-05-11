add_library(catch STATIC catch/catch.cpp)

function(pacif_test _file_)
    get_filename_component(_name_ ${_file_} NAME_WE)
    add_executable(${_name_} ${_file_})
    target_include_directories(${_name_} PRIVATE ${PROJECT_SOURCE_DIR})
    target_link_libraries(${_name_} catch)
    add_test(NAME ${_name_}
        COMMAND ${_name_}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()

file(GLOB all_test_files
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

foreach(test_file IN LISTS all_test_files)
    pacif_test(${test_file})
endforeach(test_file)
