cmake_minimum_required(VERSION 3.17)

project(Plexiglass
        VERSION 0.1
        LANGUAGES CXX)

if (MSVC)
    add_compile_options(
        /Wall /WX # All warnings and treat as errors, except...
        /wd4710 # in most standard library headers, so can't fix
        /wd4514 # in most standard library headers, so can't fix
        /wd4668 # in most standard library headers, so can't fix
        /wd4820 # can't seem to fix without adding my own padding at the end
        /wd5045 # not even sure how to fix, doesn't seem important since code is trusted
    )
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(plexlib)

add_executable(plextest
    unit-tests/source/main.cpp
    unit-tests/source/doctest.h
    unit-tests/source/read_test_file.hpp

    unit-tests/source/test_lexer.cpp
    unit-tests/source/test_parameters.cpp
    unit-tests/source/test_parser.cpp
    unit-tests/source/test_semantics.cpp
    unit-tests/source/test_template.cpp
    unit-tests/source/test_tree.cpp
)
target_link_libraries(plextest PUBLIC plexlib)

add_executable(plexiglass
    source/plexi/main.cpp
)
target_compile_features(plexiglass PUBLIC cxx_std_17)
target_link_libraries(plexiglass PUBLIC plexlib)

configure_file(
    data/template.hpp
    template.hpp
    COPYONLY
)
configure_file(
    data/template.cpp
    template.cpp
    COPYONLY
)
