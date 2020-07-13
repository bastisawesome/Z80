#ifndef Z80_H
#define Z80_H

#include <cstdint>
#include <array>

#include "types.h"

// Z80 flag offsets
// These are used when setting specific bits in the flag register
#define C_OFFSET 0
#define N_OFFSET 1
#define PV_OFFSET 2
#define X_OFFSET 3
#define H_OFFSET 4
#define Y_OFFSET 5
#define Z_OFFSET 6
#define S_OFFSET 7

class Z80 {
private:
    /* ********Registers******** */
    // General purpose registers
    pair af, bc, de, hl;
    // References to general purpose registers
    uint8_t &a = af.b.high;
    uint8_t &f = af.b.low;
    uint8_t &b = bc.b.high;
    uint8_t &c = bc.b.low;
    uint8_t &d = de.b.high;
    uint8_t &e = de.b.low;
    uint8_t &h = hl.b.high;
    uint8_t &l = hl.b.low;

    // Alternate register set
    pair afp, bcp, dep, hlp;
    uint8_t &ap = afp.b.high,
             &fp = afp.b.low,
              &bp = bcp.b.high,
               &cp = bcp.b.low,
                &dp = dep.b.high,
                 &ep = dep.b.low,
                  &hp = hlp.b.high,
                   &lp = hlp.b.low;

    // Special purpose registers
    register16_t pc, sp; // Program counter/stack pointer
    register16_t ix, iy; // Index registers
    register16_t i; // Interrupt page address register
    register8_t r; // Memory refresh register

    /* ********Flags******** */
    // Documentation provided by the Zilog Z80 hardware manual
    // Link: http://home.mit.bme.hu/~benes/oktatas/dig-jegyz_052/Z80.pdf
    flag_t flagC;  // Carry flag
    flag_t flagN;  // Add/Subtract
    flag_t flagPV; // Parity/Overflow
    flag_t flagX;  // Unused
    flag_t flagH;  // Half carry flag
    flag_t flagY;  // Unused
    flag_t flagZ;  // Zero flag
    flag_t flagS;  // Sign flag

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
    std::array<uint8_t, 0x7F> rom;

    /* ********ROM functions******** */
    void loadRomToMemory();

    /* ********CPU functions******** */
    void execInstruction();
    uint8_t readByte();
    uint8_t readByte(uint16_t counter);
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
    void addHL(uint16_t value);

    /* ********Debug functions******** */
    void print_debug();
    void warnUnsupportedOpcode();

public:
    Z80(std::array<uint8_t, 0x7F> rom);

    /* ********CPU functions******** */
    void run();

    /* ********Mem functions******** */
    std::array<uint8_t, 0xFF> getMem();
    uint8_t getMem(uint16_t index);
    void setMem(std::array<uint8_t, 0xFF> mem);
    void setMem(uint8_t index, uint8_t value);
    void addToMem(std::array<uint8_t, 0xFF> mem, uint16_t index);
};

#endif // Z80_H
