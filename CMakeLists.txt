cmake_minimum_required(VERSION 3.5)

project(malloc_finder VERSION 0.0.0.1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(GNUInstallDirs)

add_compile_options("-std=c++11")

include_directories(include/)

set(LIBRARY_TARGET_NAME ${PROJECT_NAME})

# List of header library files.
set(${LIBRARY_TARGET_NAME}_HEADERS
        include/malloc_finder/malloc_finder.h 
        include/malloc_finder/malloc_finder_definitions.h 
)

set(${LIBRARY_TARGET_NAME}_SOURCES
        src/malloc_finder.cpp
)

# Define a header-only library
add_library(${LIBRARY_TARGET_NAME} SHARED ${${LIBRARY_TARGET_NAME}_SOURCES})

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

# Specify include directories for both compilation and installation process.
# The $<INSTALL_PREFIX> generator expression is useful to ensure to create 
# relocatable configuration files, see https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-relocatable-packages
target_include_directories(${LIBRARY_TARGET_NAME} INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# If you used find_package() you need to use target_include_directories() and/or
# target_link_libraries(). As explained previously, depending on the imported
# objects, you may need to call either or both:
# - with imported variable:
#   target_include_directories(${LIBRARY_TARGET_NAME} ${FooPackage_INCLUDE_DIRS})
#   target_link_libraries(${LIBRARY_TARGET_NAME} ${FooPackage_LIBRARIES})
# - with imported target:
#   target_link_libraries(${LIBRARY_TARGET_NAME} FooPackage_LIBRARIES::FooPackage_LIBRARIES)

# Specify installation targets, typology and destination folders.
install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev)

install(DIRECTORY include/${PROJECT_NAME}/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h*"
    PATTERN ".svn" EXCLUDE
)

include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            EXPORT ${PROJECT_NAME}
                            VARS_PREFIX ${PROJECT_NAME}
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)
# Add the uninstall target
include(AddUninstallTarget)



enable_testing()
add_custom_target(test_verbose ${CMAKE_CTEST_COMMAND} -V)
add_subdirectory(tests)