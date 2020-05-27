#include "z80.h"

Z80::Z80(std::array<uint8_t, 10> rom) {
    this->rom = rom;
}

void Z80::loadRomToMemory() {
    std::copy(this->rom.begin(), this->rom.end(),
              this->mem.begin());
}

std::array<uint8_t, 20> Z80::getMem() {
    return this->mem;
}

uint8_t Z80::getMem(uint16_t index) {
    return this->mem[index];
}


void Z80::setMem(std::array<uint8_t, 20> mem) {
    this->mem = mem;
}

void Z80::setMem(uint8_t index, uint8_t value) {
    this->mem[index] = value;
}

void Z80::addToMem(std::array<uint8_t, 20> mem,
                   uint16_t index) {
    // Temp solution until I can do better things
    for(size_t i=0; i<mem.size(); i++) {
        // Don't copy outside the size of the memory array.
        if(index + i > this->mem.size()-1)
            break;
        this->mem[index+i] = mem[i];
    }
}

void Z80::run() {

}

void Z80::execInstruction() {

}

void Z80::print_debug() {

}
