/**
 * @file Theater.cpp
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief describe the function of theater object
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Theater.h"
#include <string>
#include <vector>
#include <mutex>
#include "ErrCode.h"

using namespace BookingSystem;

ErrCode Theater::InitTheater(void)
{
    for (uint32_t i = 0; i < MAX_SEATS_IN_THEATER; i++) {
        m_seats[i] = true; // all seats are marked as empty
    }
    return ERRCODE_SUCCESS;
}

ErrCode Theater::DisplaySeatsStatus(std::vector<std::pair<uint32_t, bool>>& seats)
{
    // get status is risk free, because it would not change the status of seats
    for (uint32_t i = 0; i < MAX_SEATS_IN_THEATER; i++) {
        // create the pair with one by one value given
        seats.emplace_back(std::make_pair(i, m_seats[i]));
    }
    return ERRCODE_SUCCESS;
}

ErrCode Theater::BookSeats(const std::set<uint32_t>& seats)
{
    // book seats is risky function, that it is changing the status of seats
    const std::lock_guard<std::mutex> lock(m_theaterLockMutex);
    std::set<uint32_t>::iterator iter;
    bool isAvailable = false;
    for (iter = seats.begin(); iter != seats.end(); iter++) {
        if (*iter >= MAX_SEATS_IN_THEATER) {
            // check seat number is within the range
            printf("seat number %u is invalid\n", *iter);
            isAvailable = false;
            return ERRCODE_INVALID_PARAMETER;
        } else if (!m_seats[*iter]) {
            // check seat number is occupied or not
            printf("seat number %u is occupied\n", *iter);
            isAvailable = false;
            return ERRCODE_SEAT_IS_OCCUPIED;
        }
    }
    // check parameter first, otherwise it will need to rollback
    for (const auto& seat : seats) {
        m_seats[seat] = false;
    }
    return ERRCODE_SUCCESS;
}

uint32_t Theater::GetTheaterCode(void)
{
    return m_theaterCode;
}