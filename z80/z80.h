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

    uint8_t opcode; // Stored here for debug information

    /* ********CPU Information******** */
    const unsigned int _4MHZ = 4000000; // CPU clock speed
    unsigned int cycles; // Number of cycles for current instruction
    bool running = true;

    // Cycles per instruction, stored based on opcode value
    std::array<uint8_t, 0xFF> instructionCycles;
    // States used by the instructions, stored based on opcode value
    std::array<std::array<uint8_t, 0xFF>, 0xFF> tStates;

    /* ********Hardware Devices******** */
    std::array<uint8_t, 0xFF> mem;
    std::array<uint8_t, 10> rom;

    /* ********ROM functions******** */
    void loadRomToMemory();

    /* ********CPU functions******** */
    void execInstruction();
    uint16_t read2Bytes(uint16_t counter);
    void setFlagsSZPV(unsigned int i);
    bool parity(uint8_t b);
    void jump(bool flag);
    void setFlagH(uint8_t b1, uint8_t b2);
    void setFlagH(uint16_t b1, uint16_t b2);
    void setFlagHSub(uint8_t b1, uint8_t b2);
    void incr(uint8_t &b);
    void decr(uint8_t &b);

    /* ********Register functions******** */
    uint16_t combineRegisters(register8_t a, register8_t b);
    void addHL(uint16_t value);
    void setBC(uint16_t value);
    uint16_t getBC();
    void setDE(uint16_t value);
    uint16_t getDE();
    void setHL(uint16_t value);
    uint16_t getHL();
    void setAF(uint16_t value);
    uint16_t getAF();
    void setBCP(uint16_t value);
    uint16_t getBCP();
    void setDEP(uint16_t value);
    uint16_t getDEP();
    void setHLP(uint16_t value);
    uint16_t getHLP();
    void setAFP(uint16_t value);
    uint16_t getAFP();

    /* ********Debug functions******** */
    void print_debug();
    void warnUnsupportedOpcode(uint8_t opcode);

public:
    Z80(std::array<uint8_t, 10> rom);

    /* ********CPU functions******** */
    void run();

    /* ********Mem functions******** */
    std::array<uint8_t, 0xFF> getMem();
    uint8_t getMem(uint16_t index);
    void setMem(std::array<uint8_t, 0xFF> mem);
    void setMem(uint8_t index, uint8_t value);
    void addToMem(std::array<uint8_t, 20> mem, uint16_t index);
};

#endif // Z80_H
