cmake_minimum_required(VERSION 3.10)

project(CsvPrinter C)

enable_testing()

set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DDATA_PATH="${DATA_DIR}")

add_library(Csvprint src/csvprint.c)
target_include_directories(
    Csvprint
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
)

add_library(Tests src/tests/testCsvparse.c)
target_include_directories(
    Tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests
)

target_link_libraries(Tests PUBLIC Csvprint)

add_executable(app src/main.c)
target_link_libraries(app PUBLIC Tests)
target_link_libraries(app PUBLIC Csvprint)

add_test(
    NAME CsvParseTest
    COMMAND app --test
)