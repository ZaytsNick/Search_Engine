cmake_minimum_required(VERSION 3.28)
project(test_search_engine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(test_search_engine
        main_test.cpp
        ${SRC_DIR}/ConverterJSON.cpp
        ${SRC_DIR}/InvertedIndex.cpp
        ${SRC_DIR}/SearchServer.cpp
)

target_include_directories(test_search_engine PRIVATE ${INCLUDE_DIR})
target_link_libraries(test_search_engine PRIVATE gtest_main nlohmann_json::nlohmann_json)

configure_file(${CMAKE_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/test/config.json COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/requests.json ${CMAKE_BINARY_DIR}/test/requests.json COPYONLY)

include(GoogleTest)
gtest_discover_tests(test_search_engine)