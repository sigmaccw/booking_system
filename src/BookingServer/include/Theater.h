/**
 * @file Theater.h
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief The header to describe the functions of the object theater
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef THEATER_H
#define THEATER_H

#include <mutex>
#include <set>
#include <vector>
#include "ErrCode.h"

namespace BookingSystem {

constexpr uint32_t MAX_SEATS_IN_THEATER = 20;

class Theater {
public:
    Theater(uint32_t theaterCode) : m_theaterCode(theaterCode) {};
    
    ~Theater() {};

    ErrCode InitTheater(void);

    /**
     * @brief display seats stauts
     * 
     * @param seats [out] status of all available seats
     * @retval ERRCODE_SUCCESS - success to get status
     * @retval code that other thatn ERRCODE_SUCCESS - failed to get status
     */
    ErrCode DisplaySeatsStatus(std::vector<std::pair<uint32_t, bool>>& seats);

    /**
     * @brief book one or more seats
     * 
     * @param seats [in] the seats that is required to be reserved
     * @retval ERRCODE_SUCCESS - success to get status
     * @retval code that other thatn ERRCODE_SUCCESS - failed to get status
     */
    ErrCode BookSeats(const std::set<uint32_t>& seats);

    /**
     * @brief Get the Theater Code
     * 
     * @return theaterCode, and must be success
     */
    uint32_t GetTheaterCode();

private:
    uint32_t m_theaterCode;
    bool m_seats[MAX_SEATS_IN_THEATER];
    std::mutex m_theaterLockMutex;
};
}
#endif