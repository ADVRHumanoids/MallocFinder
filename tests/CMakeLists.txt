cmake_minimum_required(VERSION 2.8.11)
include(ExternalProject)

project(lapack_svd_tests C CXX)


# compite GTest
find_package(GTest)
find_package(Threads)
if(NOT GTEST_FOUND)
    set(gTestSource "/usr/src/gtest")
    ExternalProject_Add(GTest-ext SOURCE_DIR ${gTestSource}
                                  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/external"
                                  INSTALL_COMMAND "")
    set(GTEST_LIB_DIRS "${CMAKE_CURRENT_BINARY_DIR}/external/src/GTest-ext-build/")
    set(GTEST_INCLUDE_DIRS ${gTestSource})
    set(GTEST_BOTH_LIBRARIES gtest gtest_main)
endif()


set(TestLibs ${GTEST_BOTH_LIBRARIES} pthread)
include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${GTEST_LIB_DIRS})

add_executable(TestMallocFinder TestMallocFinder.cpp)
target_link_libraries(TestMallocFinder ${TestLibs})
add_dependencies(TestMallocFinder GTest-ext)
add_test(NAME TestMallocFinder COMMAND TestMallocFinder)


