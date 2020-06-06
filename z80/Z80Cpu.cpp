#include "z80.h"

void Z80::run() {
    while(this->running) {
        this->execInstruction();
#ifdef Z80_DEBUG
        this->print_debug();
#endif
    }
}

/*
 * Read the current program counter and execute the instruction,
 * then increment the program counter.
 */
void Z80::execInstruction() {
    this->opcode = this->mem[this->pc++];

    // Increasing CPU clock cycles will go here.

    switch(this->opcode) {
    /* ********0x00-0x0F******** */
    case 0x00:
        // NOP
        break;
    case 0x01:
        /*
         * LD BC, NN
         * Loads value NN into register pair BC
         */
        this->bc.w = this->read2Bytes(pc);
        this->pc += 2;
        break;
    case 0x02:
        /*
         * LD (BC), A
         * Stores value of A into memory address BC
         */
        this->mem[this->bc.w] = this->a;
        break;
    case 0x03:
        /*
         * INC BC
         * Increment register pair BC
         */
        this->bc.w++;
        break;
    case 0x04:
        /*
         * INC B
         * Increment register B
         */
        this->incr(this->b);
        break;
    case 0x05:
        /*
         * DEC B
         * Decrements register B
         */
        this->decr(this->b);
        break;
    case 0x06:
        /*
         * LD B, N
         * Loads byte into B
         */
        this->b = this->mem[this->pc++];
        break;
    case 0x07:
        /*
         * RLCA
         * Rotates A left one bit. Contents of bit 6 are copied to carry.
         * Flags H and N are reset.
         */
        this->flagC = ((this->a & 0x80) != 0);
        this->a = (uint8_t)((this->a << 1) | (this->a >> 7));
        this->flagH = false;
        this->flagN = false;
        break;
    case 0x08:
        /*
         * EX AF, AF'
         * Exchanges the contents of AF and AF'
         */
    {
        uint16_t tmp = this->af.w;
        this->af.w = this->afp.w;
        this->afp.w = tmp;
        break;
    }
    case 0x09:
        /*
         * ADD HL, BC
         * BC is added to HL
         */
        this->addHL(this->bc.w);
        break;
    case 0x0A:
        /*
         * LD A, (BC)
         * Loads value pointed to by BC into A
         */
        this->a = this->mem[this->bc.w];
        break;
    case 0x0B:
        /*
         * DEC BC
         * Decreases value of BC by 1
         */
        this->bc.w--;
        break;
    case 0x0C:
        /*
         * INC C
         * Increases value of C by 1
         */
        this->incr(this->c);
        break;
    case 0x0D:
        /*
         * DEC C
         * Decreases value of C by 1
         */
        this->decr(this->c);
        break;
    case 0x0E:
        /*
         * LD C, N
         * Loads byte into C
         */
        this->c = this->mem[this->pc++];
        break;
    case 0x0F:
        /*
         * RRLCA
         * The contents of A are rotated right one bit.
         * Bit 0 is copied into bit 7 and the carry flag.
         */
        this->flagC = ((this->a&0x80) != 0);
        this->a = (uint8_t)((this->a>>1) | (this->a<<7));
        this->flagH = false;
        break;

    /* ********0x10-0x1F******** */
    case 0x10:
        /*
         * DJNZ, N
         * Register B is decremented. If the value left is not zero, the signed
         * value N is added to PC.
         */
        this->b--;
        if(this->b == 0)
            this->pc++;
        else
            this->pc = this->mem[pc++];
        break;
    case 0x11:
        /*
         * LD DE, NN
         * Loads 16-bit value into register pair DE.
         */
        this->de.w = read2Bytes(this->pc);
        this->pc += 2;
        break;
    case 0x12:
        /*
         * LD (DE), A
         * Loads the value of A into memory address pointed to by DE.
         */
        this->mem[this->de.w] = this->a;
        break;

    case 0x76:
        // HALT
        // For now this kills the CPU, but in the future there will
        // be an official way to end the execution.
        // Proper behaviour on the Zilog Z80 is to freeze the CPU until it
        // receives an interrupt to resume.
        this->running = false;
        break;

    // Fall-through case for unsupported/unimplemented opcodes.
    default:
        warnUnsupportedOpcode(opcode);
        break;
    }
}

uint16_t Z80::read2Bytes(uint16_t counter) {
    uint16_t value = 0;
    value |= this->mem[counter];
    value |= this->mem[counter+1] << 8;
    return value;
}

void Z80::setFlagsSZPV(unsigned int i) {
    uint8_t b = (uint8_t)i;
    this->flagS = (b&0x80) != 0;
    this->flagZ = b == 0;
    this->flagPV = parity(b);
}

bool Z80::parity(uint8_t b) {
    uint8_t bits = 0;
    for(uint8_t i=0; i < 8; i++) {
        if((b*0x80 >> i) != 0)
            bits += 1;
    }

    return (bits % 2 == 0);
}

void Z80::jump(bool flag) {
    this->pc = (uint16_t)(flag ? read2Bytes(this->pc) : this->pc+2);
}

void Z80::setFlagH(uint8_t b1, uint8_t b2) {
    this->flagH = ((b1 & 0xf) + (b2 & 0xf)) > 0xf;
}

void Z80::setFlagH(uint16_t b1, uint16_t b2) {
    this->flagH = (((b1 & 0xff00) + (b2 & 0xff00)) & 0x800) == 0x800;
}

void Z80::setFlagHSub(uint8_t b1, uint8_t b2) {
    this->flagH = (b2 & 0xf) <= (b1 & 0xf);
}

void Z80::incr(uint8_t &b) {
    uint16_t result = b+1;
    this->setFlagH(b, 1);
    this->setFlagsSZPV(result);
    b = (uint8_t)result;
}

void Z80::decr(uint8_t &b) {
    uint16_t result = b - 1;
    this->setFlagHSub(b, 1);
    this->setFlagsSZPV(result);
    b = (uint8_t)result;
}

void Z80::addHL(uint16_t value) {
    unsigned int result = this->hl.w + value;
    this->flagC = (result >> 16) != 0;
    this->setFlagH(this->hl.w, value);
    this->flagN = false;
    this->hl.w = ((uint16_t)result);
}
