#include "z80.h"

#include <iostream>
#include <iomanip>

#include "helpers.h"

#ifdef Z80_DEBUG
#include "ops.h"
#endif

/*
 * Constructors and helper functions for the Z80 class.
 */

Z80::Z80(std::array<uint8_t, 10> rom):
    b(0), c(0), d(0), e(0), h(0), l(0),
    a(0), f(0),
    bp(0), cp(0), dp(0), ep(0), hp(0), lp(0),
    ap(0), fp(0),
    pc(0), sp(0), ix(0), iy(0), i(0), r(0),
    flagC(0), flagN(0), flagPV(0), flagH(0), flagZ(0), flagS(0), flagX(0),
    cycles(0) {
    this->rom = rom;
    this->loadRomToMemory();
}

void Z80::loadRomToMemory() {
    std::copy(this->rom.begin(), this->rom.end(),
              this->mem.begin());
}

/*
 * Write debug information to stdout and wait for user input.
 * This function will be replaced with the memory manager in a future
 * version.
 */
#ifdef Z80_DEBUG
void Z80::print_debug() {
    std::stringstream out;
    out << std::setfill('0') << std::setw(2) << std::right << std::hex;

    // Header
    out << "Z80 Debug Information:\n";

    // Output memory map
    out << "     | ";
    for(int i=0; i<=0xf; i++) {
        out << printHex(i);
        if(i < 0xf) out << " | ";
    }
    out << "\n";
    for(size_t i=0; i<this->mem.size(); i+=0xf) {
        out << printHex(i / 0xf) << " | ";
        for(unsigned int j=i; j<=0xf+i && j<this->mem.size(); j++) {
            out << printHex(this->mem[j]);
            if(j != 0xf+i && j != this->mem.size()-1) out << " | ";
        }
        out << "\n";
    }

    // Add padding
    out << "\n";

    // Output register information
    // General purpose registers
    out << "Register B: " << printHex(this->b) << "\n";
    out << "Register C: " << printHex(this->c) << "\n";
    out << "Register D: " << printHex(this->d) << "\n";
    out << "Register E: " << printHex(this->e) << "\n";
    out << "Register H: " << printHex(this->h) << "\n";
    out << "Register L: " << printHex(this->l) << "\n";

    // Register pairs
    out << "Register BC: " << printHex(combineRegisters(this->b, this->c), 4)
        << "\n";
    out << "Register DE: " << printHex(combineRegisters(this->d, this->e), 4)
        << "\n";
    out << "Register HL: " << printHex(combineRegisters(this->h, this->l), 4)
        << "\n";

    // Alternate registers
    out << "Alt Register B: " << printHex(this->bp) << "\n";
    out << "Alt Register C: " << printHex(this->cp) << "\n";
    out << "Alt Register D: " << printHex(this->dp) << "\n";
    out << "Alt Register E: " << printHex(this->ep) << "\n";
    out << "Alt Register H: " << printHex(this->hp) << "\n";
    out << "Alt Register L: " << printHex(this->lp) << "\n";

    // Alternate register pairs
    out << "Alt Register BC: " << printHex(combineRegisters(this->bp, this->cp), 4)
        << "\n";
    out << "Alt Register DE: " << printHex(combineRegisters(this->dp, this->ep), 4)
        << "\n";
    out << "Alt Register HL: " << printHex(combineRegisters(this->hp, this->lp), 4)
        << "\n";

    // Special-purpose registers
    out << "Accumulator: " << printHex(this->a) << "\n";
    out << "Flags: " << printHex(this->f) << "\n";
    out << "\tC: " << printHex(this->flagC) << "\n";
    out << "\tN: " << printHex(this->flagN) << "\n";
    out << "\tPV: " << printHex(this->flagPV) << "\n";
    out << "\tH: " << printHex(this->flagH) << "\n";
    out << "\tZ: " << printHex(this->flagZ) << "\n";
    out << "\tS: " << printHex(this->flagS) << "\n";
    out << "\tX(unused): " << printHex(this->flagX) << "\n";
    out << "PC: " << printHex(this->pc, 4) << "\n";
    out << "SP: " << printHex(this->sp, 4) << "\n";
    out << "IX: " << printHex(this->ix, 4) << "\n";
    out << "IY: " << printHex(this->iy, 4) << "\n";
    out << "I: " << printHex(this->i, 4) << "\n";
    out << "R: " << printHex(this->r) << "\n";

    // Alternate special-purpose registers
    out << "Alt Accumulator: " << printHex(this->ap) << "\n";
    out << "Alt Flags: " << printHex(this->fp) << "\n";

    // Output current instruction
    out << "Current instruction: " << opNames[this->opcode];
    out << " (" << printHex(this->opcode) << ")\n";

    out << "\n";

    std::cout << out.str();

    // Pause until user enters
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
#endif

void Z80::warnUnsupportedOpcode(uint8_t opcode) {
    // Setup formatting for the program counter
    std::cout << std::hex << std::showbase << std::internal <<
              std::setfill('0');
    std::cout << std::setw(6) << this->pc-1 << " Unsupported operation " <<
              std::setw(4) << +opcode << "\n";

    // Reset formatting
    std::cout << std::dec;
}
