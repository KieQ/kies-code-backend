cmake_minimum_required(VERSION 3.16)

#Create the library
add_library(lib INTERFACE)

target_include_directories(lib INTERFACE cinatra/include)
target_link_libraries(lib INTERFACE pthread)
