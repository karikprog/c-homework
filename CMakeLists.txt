cmake_minimum_required(VERSION 3.10)


project(graph C)

enable_testing()

add_library(Graph src/graph.c)


add_executable(app src/main.c)
target_link_libraries(app PUBLIC Graph)

add_executable(runTests src/tests.c)
target_link_libraries(runTests PUBLIC Graph)

add_test(NAME BasicTests COMMAND runTests)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/city.txt" 
    "${CMAKE_CURRENT_BINARY_DIR}/city.txt" 
    COPYONLY
)