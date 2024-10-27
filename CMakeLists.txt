cmake_minimum_required(VERSION 3.17.0)
# setup project and load toolchain
project(BOOKING_SYSTEMS LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "/Library/Developer/CommandLineTools/usr/bin/g++")
set(CMAKE_OSX_ARCHITECTURES x86_64 CACHE INTERNAL "" FORCE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

# setting top global variable
set(BOOKING_SYSTEMS_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(CMAKE_INCLUDE_PATH ${BOOKING_SYSTEMS_ROOT_DIR}/include)
set(CMAKE_BINARY_DIR build)
set(CMAKE_INSTALL_PREFIX ${BOOKING_SYSTEMS_ROOT_DIR}/output)

# adding external dependency
find_package(GTest REQUIRED)

message(STATUS "GTest_DIR = ${GTest_DIR}")
message(STATUS "GTest_INCLUDE_DIRS = ${GTest_INCLUDE_DIRS}")

# added subdirectory
add_subdirectory(src)
#add_subdirectory(test)

# install
install(TARGETS system_core LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS booking_server RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS booking_client RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
#install(TARGETS booking_system_test RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/test)