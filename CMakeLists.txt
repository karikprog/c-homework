cmake_minimum_required(VERSION 3.10)

project(AvlTree C)

enable_testing()

add_library(Avl src/avl.c)
target_include_directories(
    Avl
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src 
)

add_library(Tests src/tests/tests.c)
target_include_directories(
    Tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tests 
)

target_link_libraries(Tests PUBLIC Avl)
add_executable(app src/main.c)
target_compile_options(app PRIVATE 
    -Wall             
    -Wextra           
    -Wpedantic                
)

target_link_libraries(app PUBLIC Avl)
target_link_libraries(app PUBLIC Tests)

add_test(
    NAME AvlTreeTests
    COMMAND app --test
)