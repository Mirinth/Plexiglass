add_library(plexlib STATIC)
target_sources(plexlib
    PUBLIC
    source/error.hpp
    source/plexlib.hpp
    source/utils.hpp
    source/lexer/lexer.hpp
    source/parser/parser.hpp
    source/parser/tree.hpp
    source/analyzer/analyzer.hpp
    source/template/template.hpp

    PRIVATE
    source/error.cpp
    source/plexlib.cpp
    source/utils.cpp
    source/lexer/lexer.cpp
    source/parser/parser.cpp
    source/parser/tree.cpp
    source/analyzer/analyzer.cpp
    source/template/template.cpp
)
target_include_directories(plexlib PUBLIC source)
target_compile_features(plexlib PUBLIC cxx_std_17)

file(READ templates/template.hpp PLEXLIB_HEADER_TEMPLATE_CONTENT)
file(READ templates/template.cpp PLEXLIB_CODE_TEMPLATE_CONTENT)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(
    templates/template-holder.hpp
    template-holder.hpp
)
