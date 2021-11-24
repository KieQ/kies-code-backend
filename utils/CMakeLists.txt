cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS)

add_executable(any_test any_test.cpp any.cpp)
target_link_libraries(any_test PRIVATE ${CONAN_LIBS})

add_executable(json_test json_test.cpp json.cpp)
target_link_libraries(json_test PRIVATE ${CONAN_LIBS})

add_test(any_test any_test)
add_test(json_test json_test)

