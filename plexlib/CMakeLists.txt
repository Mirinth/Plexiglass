add_library(plexlib STATIC)
target_sources(plexlib
    PUBLIC
    source/error.hpp
    source/plexlib.hpp
    source/utils.hpp
    source/lexer/lexer.hpp
    source/lexer/token.hpp
    source/parser/parser.hpp
    source/parser/tree.hpp
    source/analyzer/analyzer.hpp
    source/template/template.hpp

    PRIVATE
    source/error.cpp
    source/plexlib.cpp
    source/utils.cpp
    source/lexer/lexer.cpp
    source/lexer/token.cpp
    source/parser/parser.cpp
    source/parser/tree.cpp
    source/analyzer/analyzer.cpp
    source/template/template.cpp
)
target_include_directories(plexlib PUBLIC source)
target_compile_features(plexlib PUBLIC cxx_std_17)
