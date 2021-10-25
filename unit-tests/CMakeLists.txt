add_executable(plextest
    source/main.cpp
    source/doctest.h
    source/test_files.hpp

    source/test_lexer.cpp
    source/test_parameters.cpp
    source/test_parser.cpp
    source/test_semantics.cpp
    source/test_template.cpp
    source/test_tree.cpp
)
target_link_libraries(plextest PUBLIC plexlib)
target_compile_features(plextest PUBLIC cxx_std_17)
