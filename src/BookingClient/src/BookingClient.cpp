#include <cstring>
#include <iostream>
#include <regex>
#include <set>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ErrCode.h"

static uint32_t DisplayAllMovie(uint32_t socket)
{
    std::string req = "m";
    printf("sending request = %s\n", req.c_str());
    if (send(socket, req.c_str(), req.size(), 0) < 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    char resp[256] = {0};
    int respLength = recv(socket, resp, sizeof(resp), 0);
    if (respLength <= 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    // printf("Message from server:, length = %d, content = %s\n", respLength, resp);
    // reply message: errorcode + data
    std::string respStr(resp);
    std::regex delimiter(",");
    std::sregex_token_iterator it(respStr.begin(), respStr.end(), delimiter, -1);
    std::sregex_token_iterator end;
    ErrCode ret = ERRCODE_SUCCESS;
    if (it != end) { // ErrCode should be passed first
        ret = std::stoul(*it, nullptr, 0);
        it++;
    }
    if (ret == ERRCODE_SUCCESS) {
        std::set<uint32_t> movies;
        while(it != end) { // add movie code one by one
            movies.insert(std::stoul(*it, nullptr, 0));
            it++;
        }
        printf("There are %zu movies, they are ", movies.size());
        for (auto& movie : movies) {
            printf("%u ", movie);
        }
        printf("\n");
    }
    return ERRCODE_SUCCESS;
}

static uint32_t DisplayAllTheatersWithSelectedMovie(uint32_t socket)
{
    printf("please enter the movie code: ");
    uint32_t movieCode = 0;
    scanf("%u", &movieCode);
    printf("\n"); // an extra line can make the printing more clear
    std::string req = "t," + std::to_string(movieCode);
    if (send(socket, req.c_str(), req.size(), 0) < 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    char resp[256] = {0};
    int respLength = recv(socket, resp, sizeof(resp), 0);
    if (respLength <= 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    // printf("Message from server: %s, length = %d\n", resp, respLength);
    // reply message: errorcode + data, 
    std::string respStr(resp);
    std::regex delimiter(",");
    std::sregex_token_iterator it(respStr.begin(), respStr.end(), delimiter, -1);
    std::sregex_token_iterator end;
    ErrCode ret = ERRCODE_SUCCESS;
    if (it != end) { // ErrCode should be passed first
        ret = std::stoul(*it, nullptr, 0);
        it++;
    }
    if (ret == ERRCODE_SUCCESS) {
        std::set<uint32_t> theaters;
        while(it != end) { // seats number set followed
            theaters.insert(std::stoul(*it, nullptr, 0));
            it++;
        }
        printf("There are %zu theaters for movie %u, they are ", theaters.size(), movieCode);
        for (auto& theater : theaters) {
            printf("%u ", theater);
        }
        printf("\n");
    }
    return ERRCODE_SUCCESS;
}

static uint32_t DisplayAllAvailableSeats(uint32_t socket)
{
    printf("please enter the movie code: ");
    uint32_t movieCode = 0;
    scanf("%u", &movieCode);
    printf("please enter the theater code: ");
    uint32_t theaterCode = 0;
    scanf("%u", &theaterCode);
    printf("\n");

    std::string req = "s," + std::to_string(movieCode) + "," + std::to_string(theaterCode);    
    if (send(socket, req.c_str(), req.size(), 0) < 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }

    char message[256] = {0};
    int messageLength = recv(socket, message, sizeof(message), 0);
    if (messageLength <= 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    // printf("Message from server: %s, length = %d\n", message, messageLength);
    // reply message: errorcode + data, 
    std::string messageStr(message);
    std::regex delimiter(",");
    std::sregex_token_iterator it(messageStr.begin(), messageStr.end(), delimiter, -1);
    std::sregex_token_iterator end;
    ErrCode ret = ERRCODE_SUCCESS;

    if (it != end) { // ErrCode should be passed first
        ret = std::stoul(*it, nullptr, 0);
        it++;
    }
    if (ret == ERRCODE_SUCCESS) {
        std::array<bool, 20> seats;
        uint32_t i = 0;
        while(it != end) { // seats number set followed
            seats[i] = (std::stoul(*it, nullptr, 0) != 0);
            i++;
            if (i >= 20) {
                break;
            }
            it++;
        }
        printf("The seats status for movie %u that in theater %u:\n", movieCode, theaterCode);
        for (uint32_t i = 0; i < 20; i++) {
            printf("[%2u]: %s\n", i + 1, (seats[i]) ? "empty" : "occupied"); // convert the seat number to 1-indexed 
        }
    }
    return ERRCODE_SUCCESS;
}

static uint32_t BookSeat(uint32_t socket)
{
    printf("please enter the movie code: ");
    uint32_t movieCode = 0;
    scanf("%u", &movieCode);
    printf("please enter the theater code: ");
    uint32_t theaterCode = 0;
    scanf("%u", &theaterCode);
    std::string req = "b," + std::to_string(movieCode) + "," + std::to_string(theaterCode);
    while(1) {
        uint32_t tmpSeat = 0;
        printf("please enter the seat wanted to be reserved [please enter 1 - 20 for corresponding seat number or 0 to exit]:\n");
        scanf("%u", &tmpSeat);
        if (tmpSeat == 0) {
            printf("finished marking seats\n");
            break;
        } else {
            if (tmpSeat > 20) {
                printf("invalid input\n");
                break;
            } else {
                printf("marked seat %u\n", tmpSeat);
            }
            req += "," + std::to_string(tmpSeat - 1);
        }
    }
    // printf("sending req to server %s\n", req.c_str()); 
    if (send(socket, req.c_str(), req.size(), 0) < 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    char message[256] = {0};
    int messageLength = recv(socket, message, sizeof(message), 0);
    if (messageLength <= 0) {
        printf("socket error\n");
        return ERRCODE_SOCKET_ERROR;
    }
    // printf("Message from server: %s, length = %d\n", message, messageLength);
    // reply message: errorcode + data, 
    std::string messageStr(message);
    ErrCode ret = ERRCODE_SUCCESS;
    ret = std::stoul(messageStr, nullptr, 0);
    if (ret == ERRCODE_SUCCESS) {
        printf("booking successed\n");
    } else {
        printf("booking failed\n");
    }
    return ret;
}

static void PrintHelp(void)
{
    printf("\nplease enter the requirement:\n"
            "b/B: book seats\n"
            "m/M: display all movies\n"
            "t/T: display all theaters for movie\n"
            "s/S: display all seats in theater for movie\n"
            "q/Q: exit program\n");
}

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr*)&serverAddress,
            sizeof(serverAddress));

    // message: opcode + data, 
    // opcode: b: book seats, data: 1st number means movieCode, 2nd number means theaterCode, 3rd - 23rd number means seats number 
    //         t: show theaters, data: 1st number means movieCode
    //         m: show movies, data: no data
    //         s: show seats, data: 1st number means movieCode, 2nd number means theaterCode
    PrintHelp();
    while(1) {
        char command = 0;
        scanf("%c", &command); // need to compatible for strange charaters
        ErrCode ret = ERRCODE_SUCCESS;
        if ((command == 'b') || (command == 'B')) {
            ret = BookSeat(clientSocket);
        } else if ((command == 't') || (command == 'T')) {
            ret = DisplayAllTheatersWithSelectedMovie(clientSocket);
        } else if ((command == 'm') || (command == 'M')) {
            ret = DisplayAllMovie(clientSocket);
        } else if ((command == 's') || (command == 'S')) {
            ret = DisplayAllAvailableSeats(clientSocket);
        } else if ((command == 'q') || (command == 'Q')) {
            printf("exiting system\n");
            break;
        } else {
            PrintHelp();
        }
    }
    // closing socket
    close(clientSocket);
    return 0;
}