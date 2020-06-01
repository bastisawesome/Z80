#include "z80.h"

std::array<uint8_t, 0xFF> Z80::getMem() {
    return this->mem;
}

uint8_t Z80::getMem(uint16_t index) {
    return this->mem[index];
}


void Z80::setMem(std::array<uint8_t, 0xFF> mem) {
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
