cmake_minimum_required(VERSION 3.28)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

add_executable(search_engine
        main.cpp
        ${SRC_DIR}/ConverterJSON.cpp
        ${SRC_DIR}/InvertedIndex.cpp
        ${SRC_DIR}/SearchServer.cpp
)

target_include_directories(search_engine PRIVATE ${INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
configure_file(requests.json ${CMAKE_BINARY_DIR}/requests.json COPYONLY)

enable_testing()
add_subdirectory(test)