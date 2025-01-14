cmake_minimum_required(VERSION 3.28)
project(test_search_engine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Подключение GoogleTest
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Определение абсолютных путей к исходникам и заголовкам
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Создание исполняемого файла для тестов
add_executable(test_search_engine
        main_test.cpp
        ${SRC_DIR}/ConverterJSON.cpp
        ${SRC_DIR}/InvertedIndex.cpp
        ${SRC_DIR}/SearchServer.cpp
)

# Подключение заголовочных файлов и библиотек
target_include_directories(test_search_engine PRIVATE ${INCLUDE_DIR})
target_link_libraries(test_search_engine PRIVATE gtest_main nlohmann_json::nlohmann_json)

configure_file(${CMAKE_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/test/config.json COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/requests.json ${CMAKE_BINARY_DIR}/test/requests.json COPYONLY)

#configure_file(config.json ${CMAKE_BINARY_DIR}/test/config.json COPYONLY)
#configure_file(requests.json ${CMAKE_BINARY_DIR}/test/requests.json COPYONLY)

# Автоматическое обнаружение тестов
include(GoogleTest)
gtest_discover_tests(test_search_engine)


#cmake_minimum_required(VERSION 3.28)
#
#project(test_search_engine)
#
#set(CMAKE_CXX_STANDARD 17)
#
#include(FetchContent)
#
#FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#set(gtest_disable_pthreads on)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
#enable_testing()
#
##set(INCLUDE_DIR ../${CMAKE_SOURCE_DIR}/include)
##set(SRC_DIR ../${CMAKE_SOURCE_DIR}/src)
#
## Создание исполняемого файла search_engine
#add_executable(test_search_engine
#        main_test.cpp
#        ${SRC_DIR}/ConverterJSON.cpp
#        ${SRC_DIR}/InvertedIndex.cpp
#        ${SRC_DIR}/SearchServer.cpp
#)
#
#target_include_directories(test_search_engine PRIVATE ${INCLUDE_DIR})
#
##configure_file(config.json ../${CMAKE_BINARY_DIR}/config.json COPYONLY)
##configure_file(requests.json ../${CMAKE_BINARY_DIR}/requests.json COPYONLY)
#
#
##target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)
#target_link_libraries(search_engine PRIVATE gtest_main)
#
#include(GoogleTest)
#gtest_discover_tests(test_search_engine)