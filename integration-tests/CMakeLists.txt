add_executable(basic-integration-test
    basic-test/lexer.hpp
	basic-test/lexer.cpp
	main.cpp
)
target_include_directories(basic-integration-test
	PRIVATE basic-test
)
add_dependencies(basic-integration-test plexiglass)
target_compile_features(basic-integration-test PUBLIC cxx_std_17)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/basic-test/lexer.cpp
           ${CMAKE_CURRENT_SOURCE_DIR}/basic-test/lexer.hpp
	COMMAND plexiglass ${CMAKE_CURRENT_SOURCE_DIR}/basic-test/lexer.txt
	MAIN_DEPENDENCY basic-test/lexer.txt
	DEPENDS plexiglass basic-test/lexer.txt
	VERBATIM
	COMMENT "Generating basic-test lexer."
)

set(LEXER_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/basic-test")

target_include_directories(basic-integration-test
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)
