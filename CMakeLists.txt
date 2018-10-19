project(svd_test)

cmake_minimum_required(VERSION 3.5)

add_compile_options("-std=c++11")

include_directories(include/)

enable_testing()
add_custom_target(test_verbose ${CMAKE_CTEST_COMMAND} -V)
add_subdirectory(tests)