# compile for test problem
add_executable(booking_system_test TestBookingSystem.cpp TestMain.cpp)
target_include_directories(booking_system_test PRIVATE ${CMAKE_INCLUDE_PATH} ${GTest_INCLUDE_DIRS})
target_link_libraries(booking_system_test system_core gtest::gtest)