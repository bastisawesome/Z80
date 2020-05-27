#ifndef TYPES_H
#define TYPES_H

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
typedef bool flag_t;

#endif // TYPES_H
