cmake_minimum_required(VERSION 3.16)
project(search_tree_research C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(trees src/bst.c src/avl.c src/splay.c)
target_include_directories(trees PUBLIC include)

add_executable(tree_benchmark src/benchmark.c)
target_link_libraries(tree_benchmark PRIVATE trees)

add_executable(tree_tests tests/test_trees.c)
target_link_libraries(tree_tests PRIVATE trees)

enable_testing()
add_test(NAME tree_tests COMMAND tree_tests)

