/**
 * @file ErrCode.h
 * @author Sigma Choi (sigmachoi@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef ERRCODE_H
#define ERRCODE_H

#include <cstdint>

typedef uint32_t ErrCode;

constexpr uint32_t ERRCODE_SUCCESS = 0x0;
constexpr uint32_t ERRCODE_INVALID_PARAMETER = 0x1;
constexpr uint32_t ERRCODE_MALLOC_FAILURE = 0x2;
constexpr uint32_t ERRCODE_NULL_POINTER = 0x3;
constexpr uint32_t ERRCODE_SEAT_IS_OCCUPIED = 0x4;
constexpr uint32_t ERRCODE_THEATER_DOES_NOT_EXIST = 0x5;
constexpr uint32_t ERRCODE_SOCKET_ERROR = 0x6;
constexpr uint32_t ERRCODE_MEMORY_ERROR = 0x7;

#endif