cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

add_executable(any_test any_test.cpp any.cpp)
target_link_libraries(any_test PRIVATE gtest::gtest)

add_executable(json_test json_test.cpp json.cpp)
target_link_libraries(json_test PRIVATE gtest::gtest nlohmann_json::nlohmann_json)

add_test(any_test any_test)
add_test(json_test json_test)

