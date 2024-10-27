/**
 * @file Movie.cpp
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief describe the function of object movie
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "Movie.h"
#include <set>
#include "ErrCode.h"

using namespace BookingSystem;

ErrCode Movie::AddTheater(const std::set<uint32_t>& newTheaters)
{
    // input parameter used set to confirm every element in it
    // is unique, so no repeat cases are required to check
    const std::lock_guard<std::mutex> lock(m_movieLockMutex);
    bool foundSameTheater = false;
    for (auto theaterCode = newTheaters.begin(); theaterCode != newTheaters.end();
         ++theaterCode) {
        if (m_theater.find(*theaterCode) != m_theater.end()) {
            // theater added in map already
            printf("theater %u added into memory already\n", *theaterCode);
            continue;
        } else {
            Theater* addTheater = new Theater(*theaterCode);
            if (addTheater == nullptr) {
                printf("cannot allocate memory\n");
                return ERRCODE_MALLOC_FAILURE;
            } else {
                m_theater.insert({*theaterCode, addTheater});
                addTheater->InitTheater();
            }
        }
    }
    return ERRCODE_SUCCESS;
}

ErrCode Movie::DelTheater(const std::set<uint32_t>& oldTheaters)
{
    const std::lock_guard<std::mutex> lock(m_movieLockMutex);
    bool foundSameTheater = false;
    std::set<uint32_t>::iterator theaterCode;
    for (auto theaterCode = oldTheaters.begin(); theaterCode != oldTheaters.end();
         ++theaterCode) {
        auto theaterIter = m_theater.find(*theaterCode);
        if (theaterIter == m_theater.end()) {
            // theater not in map
            printf("theater %u did not add into memory\n", *theaterCode);
            continue;
        } else {
            // found theater
            delete(theaterIter->second);
            theaterIter->second = nullptr;
            m_theater.erase(theaterIter);
        }
    }
    return ERRCODE_SUCCESS;
}

ErrCode Movie::ShowAllTheaters(std::set<uint32_t>& theaterCode)
{
    for (auto& iter: m_theater) {
        theaterCode.insert(iter.first); // copy the key of unordered_map to set
    }
    return ERRCODE_SUCCESS;
}

ErrCode Movie::BookSeat(const uint32_t theaterCode, const std::set<uint32_t>& seats)
{
    auto theaterIter = m_theater.find(theaterCode);
    if (theaterIter == m_theater.end()) {
        // theater not in map
        printf("theater %u did not add into memory\n", theaterIter->first);
        return ERRCODE_THEATER_DOES_NOT_EXIST;
    } else {
        // found theater
        Theater* theater = theaterIter->second;
        if (theater == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        return theater->BookSeats(seats);
    }
}

ErrCode Movie::GetSeatStatus(const uint32_t theaterCode, std::vector<std::pair<uint32_t, bool>>& seats)
{
    auto theaterIter = m_theater.find(theaterCode);
    if (theaterIter == m_theater.end()) {
        // theater not in map
        printf("theater %u did not add into memory\n", theaterCode);
        return ERRCODE_THEATER_DOES_NOT_EXIST;
    } else {
        // found theater
        Theater* theater = theaterIter->second;
        if (theater == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        return theater->DisplaySeatsStatus(seats);
    }
}