# compile for client side cmdline 
add_executable(booking_client src/BookingClient.cpp)
target_include_directories(booking_client PUBLIC ${CMAKE_INCLUDE_PATH})
target_link_libraries(booking_client pthread)