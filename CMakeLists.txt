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

add_library(plexlib STATIC
    plexlib/source/error.hpp
    plexlib/source/error.cpp
    plexlib/source/plexlib.hpp
    plexlib/source/plexlib.cpp
    plexlib/source/utils.hpp
    plexlib/source/utils.cpp
    
    plexlib/source/lexer/lexer.hpp
    plexlib/source/lexer/lexer.cpp
    plexlib/source/lexer/token.hpp
    plexlib/source/lexer/token.cpp

    plexlib/source/parser/parser.hpp
    plexlib/source/parser/parser.cpp
    plexlib/source/parser/tree.hpp
    plexlib/source/parser/tree.cpp

    plexlib/source/analyzer/analyzer.hpp
    plexlib/source/analyzer/analyzer.cpp

    plexlib/source/template/template.hpp
    plexlib/source/template/template.cpp
)
target_include_directories(plexlib PUBLIC plexlib/source)
target_compile_features(plexlib PUBLIC cxx_std_17)



# add_executable(plexi
#     source/plexi/main.cpp
#     source/plexlib/RegularExpressionParser.hpp
#     source/plexlib/RegularExpressionParser.cpp
# )
# target_link_libraries(plexi PUBLIC plexlib)


add_executable(plextest
    source/test/main.cpp
    source/test/doctest.h
    source/test/read_test_file.hpp
    
    source/test/test_lexer.cpp
    source/test/test_parameters.cpp
    source/test/test_parser.cpp
    source/test/test_semantics.cpp
    source/test/test_template.cpp
    source/test/test_tree.cpp
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
