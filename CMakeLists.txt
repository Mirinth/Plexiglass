cmake_minimum_required(VERSION 3.17)

project(Plexiglass
        VERSION 0.1
        LANGUAGES CXX)



add_library(plexlib STATIC
    source/plexlib/FiniteStateMachine.hpp
    source/plexlib/FiniteStateMachine.cpp
    source/plexlib/plexlib.hpp
    source/plexlib/plexlib.cpp
    source/plexlib/RegularExpressionParser.hpp
    source/plexlib/RegularExpressionParser.cpp
    
    source/plexlib/lexer/lexer.hpp
    source/plexlib/lexer/lexer.cpp
    source/plexlib/lexer/rules.hpp
    source/plexlib/lexer/rules.cpp
    source/plexlib/lexer/token.hpp
    source/plexlib/lexer/token.cpp
)
target_include_directories(plexlib PUBLIC source/plexlib)
target_compile_features(plexlib PUBLIC cxx_std_17)



add_executable(plexi
    source/plexi/main.cpp
    source/plexlib/RegularExpressionParser.hpp
    source/plexlib/RegularExpressionParser.cpp
)
target_link_libraries(plexi PUBLIC plexlib)


add_executable(plextest
    source/test/main.cpp
)
target_link_libraries(plextest PUBLIC plexlib)