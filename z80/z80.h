#ifndef Z80_H
#define Z80_H

#include <cstdint>
#include <array>

#include "types.h"

class Z80 {
private:
    /* ********Registers******** */
    // General purpose registers
    register8_t b, c, d, e, h, l;
    register8_t a; // Accumulator
    register8_t f; // Flag

    // Alternate register set
    register8_t bp, cp, dp, ep, hp, lp;
    register8_t ap; // Alternate accumulator
    register8_t fp; // Alternate flag

    // Special purpose registers
    register16_t pc, sp; // Program counter/stack pointer
    register16_t ix, iy; // Index registers
    register16_t i; // Interrupt page address register
    register8_t r; // Memory refresh register

    /* ********Flags******** */
    // Documentation provided by the Zilog Z80 hardware manual
    // Link: http://home.mit.bme.hu/~benes/oktatas/dig-jegyz_052/Z80.pdf
    flag_t flagC; // Carry flag
    flag_t flagN; // Add/Subtract
    flag_t flagPV; // Parity/Overflow
    flag_t flagH; // Half carry flag
    flag_t flagZ; // Zero flag
    flag_t flagS; // Sign flag
    flag_t flagX; // Unused

    /* ********CPU Information******** */
    const unsigned int _4MHZ = 4000000; // CPU clock speed
    unsigned int cycles; // Number of cycles for current instruction

    // Cycles per instruction, stored based on opcode value
    std::array<uint8_t, 0xFF> instructionCycles;
    // States used by the instructions, stored based on opcode value
    std::array<std::array<uint8_t, 0xFF>, 0xFF> tStates;

    /* ********Hardware Devices******** */
    std::array<uint8_t, 20> mem;
    std::array<uint8_t, 10> rom;

    /* ********ROM functions******** */
    void loadRomToMemory();

    /* ********CPU functions******** */
    void execInstruction();

    /* ********Debug functions******** */
    void print_debug();

public:
    Z80(std::array<uint8_t, 10> rom);

    /* ********CPU functions******** */
    void run();

    /* ********Mem functions******** */
    std::array<uint8_t, 20> getMem();
    uint8_t getMem(uint16_t index);
    void setMem(std::array<uint8_t, 20> mem);
    void setMem(uint8_t index, uint8_t value);
    void addToMem(std::array<uint8_t, 20> mem, uint16_t index);
};

#endif // Z80_H
