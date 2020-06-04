#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

/*
 * Contains definitions for datatypes as well as helper functions
 * for these datatypes.
 */

/**
 * @brief register8_t
 * Type definition for use of 8-bit registers.
 */
typedef unsigned char register8_t; // 8-bit register

/**
 * @brief register16_t
 * Type definition for use of 16-bit registers and register pairs.
 */
typedef unsigned short register16_t; // 16-bit register

/**
 * @brief flag_t
 * Type definition specifically for CPU flags.
 */
typedef unsigned char flag_t;

/**
  * @brief PAIR
  * Union to manage the register pairs used by the Z80.
  * Contains code for future support of big-endian processors.
  */
union pair {
    // Big-Endian base for architectures like PowerPC.
#ifdef BE_BASE
    struct {
        uint8_t low, high;
    } b;
    struct {
        int8_t low, high;
    } sb;
#else
    // Separate between signed and unsigned byte.
    struct {
        uint8_t high, low;
    } b;
    struct {
        int8_t high, low;
    } sb;
#endif
    // Separate between signed and unsigned word.
    uint16_t w;
    int16_t sw;
    pair(uint16_t w): w(w) {}
    ~pair() {}
};

#endif // TYPES_H
