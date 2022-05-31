cmake_minimum_required(VERSION 3.16)
project(kies)

set(CMAKE_CXX_STANDARD 17)

include(CTest)

if(LIMIT_MEM)
    message("Limit Compiler Memory Usage")
    add_compile_options("SHELL:--param ggc-min-expand=1" "SHELL:--param ggc-min-heapsize=131072")
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_compile_options(-fsanitize=address -g)
    add_link_options(-fsanitize=address)
endif()


find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
find_package(libpqxx REQUIRED)
find_package(LibtorrentRasterbar REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)



if(NOT SKIP_TEST)
    find_package(GTest REQUIRED)
    add_subdirectory(utils)
endif()

function(add_all_source src_files)
    aux_source_directory(${src_files} ${src_files})
    list(FILTER ${src_files} EXCLUDE REGEX "_test.cpp")
    set(${src_files} ${${src_files}} PARENT_SCOPE)
endfunction()

add_subdirectory(lib)

add_all_source(db)
add_all_source(handler)
add_all_source(utils)
add_all_source(service)


add_executable(kies main.cpp ${db} ${handler} ${utils} ${service})
target_link_libraries(kies PRIVATE lib boost::boost fmt::fmt libpqxx::pqxx nlohmann_json::nlohmann_json spdlog::spdlog LibtorrentRasterbar::torrent-rasterbar)

install(TARGETS kies DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/..)

