/**
 * @file Movie.h
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief The header to describe the functions of the object movie
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MOVIE_H
#define MOVIE_H

#include <mutex>
#include <set>
#include <unordered_map>
#include "ErrCode.h"
#include "Theater.h"

namespace BookingSystem {

class Movie {
public:
    Movie(uint32_t movieCode) : m_movieCode(movieCode) {}
    ~Movie() {}

    /**
     * @brief add the theater that play movie into class
     * 
     * @param theaterCode [in] the code that represent theater
     * @retval ERRCODE_SUCCESS - add theater successfully
     * @retval !ERRCODE_SUCCESS - add theater failed, check errorcode in corresponding header file
     */
    ErrCode AddTheater(const std::set<uint32_t>& newTheaters);

    /**
     * @brief delete the theater that play movie from class
     * 
     * @param theaterCode [in] the code that represent theater
     * @retval ERRCODE_SUCCESS - add theater successed
     * @retval !ERRCODE_SUCCESS - add theater failed
     */
    ErrCode DelTheater(const std::set<uint32_t>& oldTheaters);

    /**
     * @brief reply all theaters code that stored in class
     * 
     * @param theaterCode [out] the theaters that play movie
     * @retval ERRCODE_SUCCESS - delete theater successed
     * @retval !ERRCODE_SUCCESS - delete theater failed
     */
    ErrCode ShowAllTheaters(std::set<uint32_t>& theaterCode);

    /**
     * @brief book one or more than one seat(s) at specific theater
     * 
     * @param theaterCode [in] the code that represent theater
     * @param seats [in] the seats that is required to be reserved
     * @retval ERRCODE_SUCCESS - book seat successed
     * @retval !ERRCODE_SUCCESS - book seat failed
     */
    ErrCode BookSeat(const uint32_t theaterCode, const std::set<uint32_t>& seats);

    /**
     * @brief Get the Seat Status in specific theater
     * 
     * @param theaterCode [in] the code that represent theater
     * @param seats [out] status of all available seats
     * @retval ERRCODE_SUCCESS - get seat successed
     * @retval !ERRCODE_SUCCESS - get seat failed
     */
    ErrCode GetSeatStatus(const uint32_t theaterCode, std::vector<std::pair<uint32_t, bool>>& seats);

private:
    uint32_t m_movieCode; // the code that represent movie
    std::unordered_map<uint32_t, Theater*> m_theater; // the map stores the code of theaters which plays the movie
    std::mutex m_movieLockMutex;
};

}
#endif