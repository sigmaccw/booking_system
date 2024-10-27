/**
 * @file BookingServer.h
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief The header to describe the functions of booking server
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef BOOKING_SEVER_H
#define BOOKING_SEVER_H

#include <mutex>
#include <set>
#include <unordered_map>
#include "ErrCode.h"
#include "Movie.h"

constexpr uint32_t MAX_SEAT_IN_THEATER = 20;

namespace BookingSystem {
class BookingServer {
public:
    BookingServer() {}
    ~BookingServer() {}
    
    ErrCode AddMovie(uint32_t newMovie);
    ErrCode DelMovie(uint32_t oldMovie);
    ErrCode AddTheater(uint32_t movieCode, uint32_t theaterCode);
    ErrCode DelTheater(uint32_t movieCode, uint32_t theaterCode);
    ErrCode DisplayAllMovie(std::set<uint32_t>& movieCode);
    ErrCode DisplayAllTheatersWithSelectedMovie(const uint32_t movieCode, std::set<uint32_t>& theaterCode);
    ErrCode DisplayAllAvailableSeats(const uint32_t movieCode, const uint32_t theaterCode, std::array<bool, MAX_SEAT_IN_THEATER>& seats);
    ErrCode BookSeats(const uint32_t movieCode, const uint32_t theaterCode, std::set<uint32_t>seats);

private:
    std::unordered_map<uint32_t, Movie*>m_systems;
    std::mutex m_systemLockMutex;
};
}
#endif