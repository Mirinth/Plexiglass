add_executable(basic-integration-test
    source/main.cpp
)
add_dependencies(basic-integration-test plexiglass)
target_compile_features(plexlib PUBLIC cxx_std_17)
