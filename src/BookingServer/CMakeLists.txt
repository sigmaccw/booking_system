# compile for core library
add_library(system_core src/BookingServer.cpp src/Movie.cpp src/Theater.cpp)
target_include_directories(system_core PUBLIC ${CMAKE_INCLUDE_PATH}
                                       PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# compile for server side cmdline 
add_executable(booking_server src/BookingServerMain.cpp)
target_include_directories(booking_server PUBLIC ${CMAKE_INCLUDE_PATH}
                                          PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(booking_server system_core pthread)