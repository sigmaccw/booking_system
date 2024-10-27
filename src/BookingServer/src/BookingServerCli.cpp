/**
 * @file BookingServerMain.cpp
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief describe server cli and replaced by client side
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <set>
#include "ErrCode.h"
#include "BookingServer.h"

using namespace BookingSystem;

static uint32_t DisplayAllMovie(BookingServer& sys)
{
    std::set<uint32_t> movies;
    if (sys.DisplayAllMovie(movies) == ERRCODE_SUCCESS) {
        printf("There are %zu movies, they are ", movies.size());
        for (auto& movie : movies) {
            printf("%u ", movie);
        }
        printf("\n");
    } else {
        printf("display all movie failed\n");
    }
    return ERRCODE_SUCCESS;
}

static uint32_t DisplayAllTheatersWithSelectedMovie(BookingServer& sys)
{
    printf("please enter the movie code: ");
    uint32_t movieCode = 0;
    scanf("%u", &movieCode);
    printf("\n"); // an extra line can make the printing more clear
    std::set<uint32_t> theaters;
    if (sys.DisplayAllTheatersWithSelectedMovie(movieCode, theaters) == ERRCODE_SUCCESS) {
        printf("There are %zu theaters playing movie %u, they are ", theaters.size(), movieCode);
        for (auto& theater : theaters) {
            printf("%u ", theater);
        }
        printf("\n");
    } else {
        printf("display all theaters for movie %u failed\n", movieCode);
    }
    return ERRCODE_SUCCESS;
}

static uint32_t DisplayAllAvailableSeats(BookingServer& sys)
{
    printf("please enter the movie code: ");
    uint32_t movieCode = 0;
    scanf("%u", &movieCode);
    printf("please enter the theater code: ");
    uint32_t theaterCode = 0;
    scanf("%u", &theaterCode);
    printf("\n");
    std::array<bool, MAX_SEAT_IN_THEATER> seats;
    if (sys.DisplayAllAvailableSeats(movieCode, theaterCode, seats) == ERRCODE_SUCCESS) {
        printf("The seats status for movie %u that in theater %u:\n", movieCode, theaterCode);
        for (uint32_t i = 0; i < MAX_SEAT_IN_THEATER; i++) {
            printf("[%2u]: %s\n", i + 1, (seats[i]) ? "empty" : "occupied"); // convert the seat number to 1-indexed 
        }
    } else {
        printf("display all seats in theater %u for movie %u failed\n", theaterCode, movieCode);
    }
    printf("\n");
    return ERRCODE_SUCCESS;
}

static uint32_t BookSeat(BookingServer& sys)
{
    printf("please enter the movie code: ");
    uint32_t movieCode = 0;
    scanf("%u", &movieCode);
    printf("please enter the theater code: ");
    uint32_t theaterCode = 0;
    scanf("%u", &theaterCode);
    printf("\n");
    std::set<uint32_t> seats;
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
            seats.insert(tmpSeat - 1); // seats are 0-indexed
        }
    }
    if (sys.BookSeats(movieCode, theaterCode, seats) == ERRCODE_SUCCESS) {
        printf("book seat successed\n");
    } else {
        printf("book seat failed\n");
    }
    printf("\n");
    return ERRCODE_SUCCESS;
}

static void PrintHelp(void)
{
    printf("please enter the requirement:\n"
            "b/B: book seats\n"
            "m/M: display all movies\n"
            "t/T: display all theaters for movie\n"
            "s/S: display all seats in theater for movie\n"
            "q/Q: exit program\n");
}

int main()
{
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, std::array<bool, MAX_SEAT_IN_THEATER>>>systems = 
    {
        {0, {{3, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
            {4, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
            {5, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}}}
        },
        {1, {{6, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
            {7, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
            {8, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}}}
        },
        {2, {{9, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
            {10, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}},
            {11, {true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true}}}
        }
    };
    BookingServer* sys = new BookingServer(systems);
    if (sys == nullptr) {
        return ERRCODE_NULL_POINTER;
    }
    PrintHelp();
    while(1) {
        char command = 0;
        scanf("%c", &command); // need to compatible for strange charaters
        ErrCode ret = ERRCODE_SUCCESS;
        if ((command == 'b') || (command == 'B')) {
            ret = BookSeat(*sys);
        } else if ((command == 't') || (command == 'T')) {
            ret = DisplayAllTheatersWithSelectedMovie(*sys);
        } else if ((command == 'm') || (command == 'M')) {
            ret = DisplayAllMovie(*sys);
        } else if ((command == 's') || (command == 'S')) {
            ret = DisplayAllAvailableSeats(*sys);
        } else if ((command == 'q') || (command == 'Q')) {
            printf("exiting system\n");
            break;
        } else {
            PrintHelp();
        }
    }
    delete sys;
    return ERRCODE_SUCCESS;
}