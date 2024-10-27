/**
 * @file BookingServer.cpp
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief describe the server functions
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "BookingServer.h"

using namespace BookingSystem;

ErrCode BookingServer::AddMovie(uint32_t newMovie)
{
    // input parameter used set to confirm every element in it
    // is unique, so no repeat cases are required to check
    const std::lock_guard<std::mutex> lock(m_systemLockMutex);
    if (m_systems.find(newMovie) != m_systems.end()) {
        // movie added in map already
        printf("theater %u added into memory already\n", newMovie);
    } else {
        Movie* addMovie = new Movie(newMovie);
        if (addMovie == nullptr) {
            printf("cannot allocate memory\n");
            return ERRCODE_MALLOC_FAILURE;
        } else {
            m_systems.insert({newMovie, addMovie});
        }
    }
    return ERRCODE_SUCCESS;
}

ErrCode BookingServer::DelMovie(uint32_t oldMovie)
{
    const std::lock_guard<std::mutex> lock(m_systemLockMutex);
    auto movieIter = m_systems.find(oldMovie);
    if (movieIter == m_systems.end()) {
        // movie not in map
        printf("movie %u did not add into memory\n", oldMovie);
    } else {
        // found theater
        delete(movieIter->second);
        movieIter->second = nullptr;
        m_systems.erase(movieIter);
    }
    return ERRCODE_SUCCESS;
}

ErrCode BookingServer::AddTheater(uint32_t movieCode, uint32_t theaterCode)
{
    auto movieIter = m_systems.find(movieCode);
    if (movieIter != m_systems.end()) {
        Movie* movie = movieIter->second;
        if (movie == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        return movie->AddTheater({theaterCode});
    } else {
        printf("Invalid movie code\n");
        return ERRCODE_INVALID_PARAMETER;
    }
}

ErrCode BookingServer::DelTheater(uint32_t movieCode, uint32_t theaterCode)
{
    auto movieIter = m_systems.find(movieCode);
    if (movieIter != m_systems.end()) {
        Movie* movie = movieIter->second;
        if (movie == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        return movie->DelTheater({theaterCode});
    } else {
        printf("Invalid movie code\n");
        return ERRCODE_INVALID_PARAMETER;
    }
}

ErrCode BookingServer::DisplayAllMovie(std::set<uint32_t>& movieCode)
{
    for(const auto& item : m_systems) {
        movieCode.insert(item.first); // insert movieCode into set
    }
    return ERRCODE_SUCCESS;
}

ErrCode BookingServer::DisplayAllTheatersWithSelectedMovie(const uint32_t movieCode, std::set<uint32_t>& theaterCode)
{
    auto movieIter = m_systems.find(movieCode);
    if (movieIter != m_systems.end()) {
        Movie* movie = movieIter->second;
        if (movie == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        return movie->ShowAllTheaters(theaterCode);
    } else {
        printf("Invalid movie code\n");
        return ERRCODE_INVALID_PARAMETER;
    }
}

ErrCode BookingServer::DisplayAllAvailableSeats(const uint32_t movieCode, const uint32_t theaterCode, std::array<bool, MAX_SEAT_IN_THEATER>& seats)
{
    auto movieIter = m_systems.find(movieCode);
    if (movieIter != m_systems.end()) {
        Movie* movie = movieIter->second;
        if (movie == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        std::vector<std::pair<uint32_t, bool>> seatsStatus;
        ErrCode ret = movie->GetSeatStatus(theaterCode, seatsStatus);
        if (ret != ERRCODE_SUCCESS) {
            return ret;
        } else {
            for (const auto& [first, sec] : seatsStatus) {
                seats[first] = sec;
            }
        }
    }
    return ERRCODE_SUCCESS;
}

ErrCode BookingServer::BookSeats(const uint32_t movieCode, const uint32_t theaterCode, std::set<uint32_t>seats)
{
    auto movieIter = m_systems.find(movieCode);
    if (movieIter != m_systems.end()) {
        Movie* movie = movieIter->second;
        if (movie == nullptr) {
            return ERRCODE_NULL_POINTER;
        }
        std::vector<std::pair<uint32_t, bool>> seatsStatus;
        return movie->BookSeat(theaterCode, seats);
    }
    return ERRCODE_SUCCESS;
}