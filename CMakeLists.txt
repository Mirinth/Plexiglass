cmake_minimum_required(VERSION 3.17)

project(Plexiglass
        VERSION 0.1
        LANGUAGES CXX)



add_library(plexlib STATIC
    source/plexlib/plexlib.hpp
    source/plexlib/plexlib.cpp
)
target_include_directories(plexlib PUBLIC source/plexlib)
target_compile_features(plexlib PUBLIC cxx_std_17)



add_executable(plexi
    source/plexi/main.cpp
)
target_link_libraries(plexi PUBLIC plexlib)
