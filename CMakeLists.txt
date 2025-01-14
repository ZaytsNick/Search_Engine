cmake_minimum_required(VERSION 3.28)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Подключение nlohmann/json
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Создание исполняемого файла search_engine
add_executable(search_engine
        main.cpp
        ${SRC_DIR}/ConverterJSON.cpp
        ${SRC_DIR}/InvertedIndex.cpp
        ${SRC_DIR}/SearchServer.cpp
)

# Подключение заголовочных файлов и библиотек
target_include_directories(search_engine PRIVATE ${INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

# Копирование файлов конфигурации
configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
configure_file(requests.json ${CMAKE_BINARY_DIR}/requests.json COPYONLY)

# Подключение тестов
enable_testing()
add_subdirectory(test)



#cmake_minimum_required(VERSION 3.28)
#
#project(search_engine)
#
#set(CMAKE_CXX_STANDARD 17)
#
#include(FetchContent)
#FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
#FetchContent_MakeAvailable(json)
#
##FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
##include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})#set(gtest_disable_pthreads on)
##set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)#FetchContent_MakeAvailable(googletest)
##enable_testing()
#
## Указываем путь к заголовочным файлам nlohmann_json
## Добавление пути к заголовочным файлам nlohmann_json
##target_include_directories(search_engine PRIVATE ${nlohmann_json_SOURCE_DIR})
#set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
#set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
#
## Создание исполняемого файла search_engine
#add_executable(search_engine
#        main.cpp
#        ${SRC_DIR}/ConverterJSON.cpp
#        ${SRC_DIR}/InvertedIndex.cpp
#        ${SRC_DIR}/SearchServer.cpp
#)
#
#
#target_include_directories(search_engine PRIVATE ${INCLUDE_DIR})
#
#configure_file(config.json ${CMAKE_BINARY_DIR}/config.json COPYONLY)
#configure_file(requests.json ${CMAKE_BINARY_DIR}/requests.json COPYONLY)
#
##target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)
#target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
##include(GoogleTest)
#
#enable_testing()
#add_subdirectory(test)
##gtest_discover_tests(search_engine)