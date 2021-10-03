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
    source/plexlib/error.hpp
    source/plexlib/error.cpp
    source/plexlib/utils.hpp
    source/plexlib/utils.cpp
    
    source/plexlib/lexer/lexer.hpp
    source/plexlib/lexer/lexer.cpp
    source/plexlib/lexer/token.hpp
    source/plexlib/lexer/token.cpp

    source/plexlib/parser/parser.hpp
    source/plexlib/parser/parser.cpp
    source/plexlib/parser/tree.hpp
    source/plexlib/parser/tree.cpp

    source/plexlib/analyzer/analyzer.hpp
    source/plexlib/analyzer/analyzer.cpp

    source/plexlib/template/template.hpp
    source/plexlib/template/template.cpp
)
target_include_directories(plexlib PUBLIC source/plexlib)
target_compile_features(plexlib PUBLIC cxx_std_17)



# add_executable(plexi
#     source/plexi/main.cpp
#     source/plexlib/RegularExpressionParser.hpp
#     source/plexlib/RegularExpressionParser.cpp
# )
# target_link_libraries(plexi PUBLIC plexlib)


add_executable(plextest
    source/test/main.cpp
)
target_link_libraries(plextest PUBLIC plexlib)

add_executable(plexiglass
    source/plexi/main.cpp
)

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
