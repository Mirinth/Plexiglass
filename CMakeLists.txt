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
add_subdirectory(unit-tests)
add_subdirectory(plexi)
