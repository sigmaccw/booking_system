# booking_system
This is a project to describe the ticket booking system

# Getting Started
This is a C++ project, user can use g++ to compile it, and there is an external library, gtest, required.
The gtest will be downloaded by conan, so users may required to install conan manually.
To compile the source code, just type
```
./build.sh
```
and everything should be done automatically.

# Usage
There are two executable
1. booking_server
2. booking_client

booking_server is working on the server side that is able to handle 10 connections, and it will accept the connection from booking_client.
booking_client is a client side that accept the command from user and pack it in a message frame, the message frame will send to the booking_server and handle the request.
After finishing the request, booking_server will reply a response to booking_client, and the whole communication is finished.
