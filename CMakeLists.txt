cmake_minimum_required(VERSION 3.28)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

#add_executable(search_engine main.cpp)
#configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
#target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
# Adding the executable
add_executable(search_engine main.cpp)

# Copying config.json && requests.json to the build directory
configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
configure_file(requests.json ${CMAKE_BINARY_DIR}/requests.json COPYONLY)

# Linking the JSON library to the executable
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

#include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)