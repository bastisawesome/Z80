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
        this->b = readByte();
        break;
    case 0x07:
        /*
         * RLCA
         * Rotates A left one bit. Contents of bit 7 are copied to carry.
         * Flags H and N are reset.
         */
        this->a = (this->a << 1) | (this->a >> 7);
        this->f = (this->f & (this->flagS | this->flagZ | this->flagPV)) |
                  (this->a & (this->flagY | this->flagX | this->flagC));
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
        this->a = readByte(this->bc.w);
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
        this->c = readByte();
        break;
    case 0x0F:
        /*
         * RRCA
         * The contents of A are rotated right one bit.
         * Bit 0 is copied into bit 7 and the carry flag.
         * Flags N and H are reset.
         */
        this->a = (uint8_t)((this->a>>1) | (this->a<<7));

        /* flag f:
         * 0: C -> A bit 0
         * 1: N
         * 2: P/V
         * 3: X
         * 4: H reset
         * 5: X
         * 6: Z
         * 7: S
         */
        this->f = (this->f & (this->flagPV | this->flagS | this->flagZ)) |
                  (this->a & (this->flagC << 8));
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
            this->pc += readByte();
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
    case 0x13:
        /*
         * INC DE
         * Increases the value of register pair DE by 1.
         */
        this->de.w--;
        break;
    case 0x14:
        /*
         * INC D
         * Increases the value of register D by 1.
         */
        this->incr(this->d);
        break;
    case 0x15:
        /*
         * DEC D
         * Decreases the value of register D by 1.
         */
        this->decr(this->d);
        break;
    case 0x16:
        /*
         * LD D, N
         * Loads byte into D
         */
        this->d = readByte();
        break;
    case 0x17:
        /*
         * RLA
         * Rotates register A left and copies bit 7 into the carry flag
         * and the previous value of the carry flag into bit 0.
         */
    {
        uint8_t prev_carry = (this->f & this->flagC);
        this->f = this->f & (((this->a & 0x80) ? this->flagC : 0) |
                             this->flagS | this->flagZ | this->flagPV);
        this->a = ((uint8_t)(this->a << 1) | (prev_carry));
    }
    break;
    case 0x18:
        /*
         * JR N
         * Adds signed byte N to to the program counter.
         */
        this->pc += readByte();
        break;
    case 0x19:
        /*
         * ADD HL, DE
         * The value of DE is added to HL
         */
        this->addHL(this->de.w);
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
        warnUnsupportedOpcode();
        break;
    }
}

uint8_t Z80::readByte() {
    uint8_t value = 0;
    value = this->mem[this->pc++];
    return value;
}

uint8_t Z80::readByte(uint16_t counter) {
    uint8_t value = 0;
    value = this->mem[counter];
    return value;
}

uint16_t Z80::read2Bytes(uint16_t counter) {
    uint16_t value = 0;
    value |= this->mem[counter];
    value |= this->mem[counter+1] << 8;
    return value;
}

void Z80::setFlagsSZPV(unsigned int i) {
    uint8_t b = (uint8_t)i;
    this->f = (this->f & (((b&0x80)!=0) << 7)); // Set flag S
    this->f = (this->f & ((b==0) << 6));        // Set flag Z
    this->f = (this->f & ((parity(b)) << 2));   // Set flag PV
//    this->flagS = (b&0x80) != 0;
//    this->flagZ = b == 0;
//    this->flagPV = parity(b);
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
    this->f |= (((b1 & 0xf) + (b2 & 0xf)) > 0xf) << 4;
}

void Z80::setFlagH(uint16_t b1, uint16_t b2) {
    this->f |= ((((b1 & 0xff00) + (b2 & 0xff00)) & 0x800) == 0x800) << 4;
}

void Z80::setFlagHSub(uint8_t b1, uint8_t b2) {
    this->f |= ((b2 & 0xf) <= (b1 & 0xf)) << 4;
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
    this->f |= ((result >> 16) != 0);
    this->setFlagH(this->hl.w, value);
    this->flagN = false;
    this->hl.w = ((uint16_t)result);
}
