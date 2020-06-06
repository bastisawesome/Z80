#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <iomanip>
#include <string>

#include "types.h"

std::string printHex(uint16_t num, uint16_t width=2) {
    std::stringstream out;

    out << std::setfill('0') << std::right << std::hex;
    out << "0x" << std::setw(width) << num;

    return out.str();
}

std::string printHex(pair num, uint16_t width=2) {
    return printHex(num.w, width);
}

#endif // HELPERS_H
