add_executable(plextest
    test_files.hpp    
    source/main.cpp
    source/doctest.h

    source/test_lexer.cpp
    source/test_parameters.cpp
    source/test_parser.cpp
    source/test_semantics.cpp
    source/test_templater.cpp
    source/test_tree.cpp
)
target_link_libraries(plextest PUBLIC plexlib)
target_compile_features(plextest PUBLIC cxx_std_17)

get_filename_component(PLEXTEST_UNIT_TEST_DIR test-files ABSOLUTE)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(
    source/test_files.hpp
    test_files.hpp
)
