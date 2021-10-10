add_executable(plexiglass
    source/main.cpp
)
target_compile_features(plexiglass PUBLIC cxx_std_17)
target_link_libraries(plexiglass PUBLIC plexlib)
