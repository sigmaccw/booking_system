/**
 * @file BookingServerMain.cpp
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief describe server main control code
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <regex>
#include <sys/socket.h>
#include <string>
#include <set>
#include <unistd.h>
#include <mutex>
#include <list>
#include "ErrCode.h"
#include "BookingServer.h"

using namespace BookingSystem;

std::list<pthread_t> g_tid; // support 10 users only 
std::mutex g_thread_mutex;

static uint32_t DisplayAllMovie(BookingServer& sys, uint32_t socket)
{
    std::set<uint32_t> movies;
    std::string replyMessage;
    ErrCode ret = sys.DisplayAllMovie(movies);
    replyMessage += std::to_string(ret);
    if (ret == ERRCODE_SUCCESS) {
        for (const auto& movie : movies) {
            replyMessage += "," + std::to_string(movie);
        }
    }
    // printf("reply message %s\n", replyMessage.c_str());
    if (send(socket, replyMessage.c_str(), replyMessage.length(), 0) < 0) {
        return ERRCODE_SOCKET_ERROR;
    }
    return ret;
}

static uint32_t DisplayAllTheatersWithSelectedMovie(BookingServer& sys, uint32_t socket, uint32_t movieCode)
{
    std::set<uint32_t> theaters;
    std::string replyMessage;
    ErrCode ret = sys.DisplayAllTheatersWithSelectedMovie(movieCode, theaters);
    replyMessage += std::to_string(ret);
    if (ret == ERRCODE_SUCCESS) {
        for (const auto& theater : theaters) {
            replyMessage += "," + std::to_string(theater);
        }
    }
    // printf("reply message %s\n", replyMessage.c_str());
    if (send(socket, replyMessage.c_str(), replyMessage.length(), 0) < 0) {
        return ERRCODE_SOCKET_ERROR;
    }
    
    return ret;
}

static uint32_t DisplayAllAvailableSeats(BookingServer& sys, uint32_t socket, uint32_t movieCode, uint32_t theaterCode)
{
    std::array<bool, MAX_SEAT_IN_THEATER> seats;
    std::string replyMessage;
    ErrCode ret = sys.DisplayAllAvailableSeats(movieCode, theaterCode, seats);
    replyMessage += std::to_string(ret);
    if (ret == ERRCODE_SUCCESS) {
        for (uint32_t i = 0; i < MAX_SEAT_IN_THEATER; i++) {
            replyMessage += "," + std::to_string(seats[i]);
        }
    }
    // printf("reply message %s\n", replyMessage.c_str());
    if (send(socket, replyMessage.c_str(), replyMessage.length(), 0) < 0) {
        return ERRCODE_SOCKET_ERROR;
    }
    return ret;
}

static uint32_t BookSeat(BookingServer& sys, uint32_t socket, uint32_t movieCode, uint32_t theaterCode, std::set<uint32_t>& seats)
{
    std::string replyMessage;
    ErrCode ret = sys.BookSeats(movieCode, theaterCode, seats);
    replyMessage += std::to_string(ret);
    // printf("reply message %s\n", replyMessage.c_str());
    if (send(socket, replyMessage.c_str(), replyMessage.length(), 0) < 0) {
        return ERRCODE_SOCKET_ERROR;
    }
    return ret;
}

typedef struct PthreadParam {
    int socket;
    BookingServer* sys;
    uint32_t threadIndex;
} PthreadParam;

void* bookingAgent(void* param)
{
    char message[1024] = { 0 };
    ssize_t messageLength = 0;
    if (param == nullptr) {
        return nullptr;
    }
    PthreadParam* tparam = static_cast<PthreadParam*>(param);
    uint32_t clientSocket = tparam->socket;
    while(1) {
        memset(message, 0, sizeof(message));
        messageLength = recv(clientSocket, message, sizeof(message), 0);
        if (messageLength > 0) {
            // printf("Message from client: %s, length = %ld\n", message, messageLength);
            ErrCode ret = ERRCODE_SUCCESS;
            // message: opcode + data, 
            // opcode: b: book seats, data: 1st number means movieCode, 2nd number means theaterCode, 3rd - 23rd number means seats number 
            //         t: show theaters, data: 1st number means movieCode
            //         m: show movies, data: no data
            //         s: show seats, data: 1st number means movieCode, 2nd number means theaterCode

            std::string messageStr(message);
            std::regex delimiter(",");
            std::sregex_token_iterator it(messageStr.begin(), messageStr.end(), delimiter, -1);
            std::sregex_token_iterator end;
            if (it != end) {
                it++; // skip the opcode
            }
            if ((message[0] == 'b') || (message[0] == 'B')) {
                uint32_t movieCode = UINT32_MAX;
                uint32_t theaterCode = UINT32_MAX;
                std::set<uint32_t> seats;
                uint32_t i = 0;
                while(it != end) { // seats number set followed
                    if (i == 0) {  // movieCode should be passed first
                        movieCode = std::stoul(*it, nullptr, 0);
                    } else if (i == 1) { // followed by theaterCode
                        theaterCode = std::stoul(*it, nullptr, 0);
                    } else {
                        seats.insert(std::stoul(*it, nullptr, 0));
                    }
                    it++;
                    i++;
                }
                if (BookSeat(*(tparam->sys), clientSocket, movieCode, theaterCode, seats) != ERRCODE_SUCCESS) {
                    printf("BookSeat failed\n");
                }
            } else if ((message[0] == 't') || (message[0] == 'T')) {
                uint32_t movieCode = UINT32_MAX;
                if (it != end) { // movieCode should be passed first
                    movieCode = std::stoul(*it, nullptr, 0);
                    it++;
                }
                if (DisplayAllTheatersWithSelectedMovie(*(tparam->sys), clientSocket, movieCode) != ERRCODE_SUCCESS) {
                    printf("display all theaters for movie %u failed\n", movieCode);
                }
            } else if ((message[0] == 'm') || (message[0] == 'M')) {
                if (DisplayAllMovie(*(tparam->sys), clientSocket) != ERRCODE_SUCCESS) {
                    printf("display all movies failed\n");
                }
            } else if ((message[0] == 's') || (message[0] == 'S')) {
                uint32_t movieCode = UINT32_MAX;
                uint32_t theaterCode = UINT32_MAX;
                std::set<uint32_t> seats;
                if (it != end) { // movieCode should be passed first
                    movieCode = std::stoul(*it, nullptr, 0);
                    it++;
                }
                if (it != end) { // followed by theaterCode
                    theaterCode = std::stoul(*it, nullptr, 0);
                    it++;
                }
                if (DisplayAllAvailableSeats(*(tparam->sys), clientSocket, movieCode, theaterCode) != ERRCODE_SUCCESS) {
                    printf("display all seats status for movie %u, theater %u failed\n", movieCode, theaterCode);
                }
            }
        } else {
            printf("Message error\n");
            break;
        }
    }

    g_thread_mutex.lock();
    close(clientSocket);
    std::list<pthread_t>::iterator iter = g_tid.begin();
    std::advance(iter, tparam->threadIndex);
    g_tid.erase(iter);
    g_thread_mutex.unlock();
    pthread_exit(nullptr);

    return nullptr;
}

static ErrCode CreateEnv(BookingServer& sys, uint32_t numberOfMovie, uint32_t numberOfTheaterPerMovie)
{
    // add movies into system
    for (uint32_t movieCode = 0; movieCode < numberOfMovie; movieCode++) {
        if (sys.AddMovie(movieCode) != ERRCODE_SUCCESS) {
            return ERRCODE_MEMORY_ERROR;
        }
        // add theater into system
        for (uint32_t theaterCode = 0; theaterCode < numberOfTheaterPerMovie; theaterCode++) {
            uint32_t theaterTargetCode = movieCode * numberOfMovie + theaterCode;
            if (sys.AddTheater(movieCode, theaterTargetCode) != ERRCODE_SUCCESS) {
                return ERRCODE_MEMORY_ERROR;
            }
        }
    }
    return ERRCODE_SUCCESS;
}

int main()
{
    BookingServer* sys = new BookingServer();
    if (sys == nullptr) {
        return ERRCODE_NULL_POINTER;
    }
    const uint32_t movieNumber = 3;
    const uint32_t theaterNumber = 3;
    ErrCode ret = CreateEnv(*sys, movieNumber, theaterNumber);
    if (ret != ERRCODE_SUCCESS) {
        return ret;
    }
    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printf("server create failed\n");
        return ERRCODE_NULL_POINTER;
    }

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 10);

    while(1) {
        if (g_tid.size() >= 10) {
            sleep(1);
            continue; // do not allow to add new user because thread is full
        }

        int clientSocket = accept(serverSocket, nullptr, nullptr);

        g_thread_mutex.lock();

        pthread_t tid;
        PthreadParam param{clientSocket, sys, static_cast<uint32_t>(g_tid.size())};
        if (pthread_create(&tid, nullptr, bookingAgent, &param) != 0) {
            printf("Create agent for client failed\n");
        } else {
            if (pthread_detach(tid) != 0) {
                printf("thread join failed\n");
            } else {
                printf("thread created\n");
                g_tid.emplace_back(tid);
            }
        }

        g_thread_mutex.unlock();

    }

    // closing the socket.
    close(serverSocket);

    return ERRCODE_SUCCESS;
}