#include "z80.h"

/*
 * Takes two registers, a and b, and combines them to create a 16-bit value.
 * Register a is the upper 8-bits and register b is the lower 8-bits.
 */
uint16_t Z80::combineRegisters(register8_t a, register8_t b) {
    uint16_t value = ((a << 8) | b);
    return value;
}

void Z80::addHL(uint16_t value) {
    unsigned int result = this->getHL() + value;
    this->flagC = (result >> 16) != 0;
    this->setFlagH(this->getHL(), value);
    this->flagN = false;
    this->setHL((uint16_t)result);
}

void Z80::setBC(uint16_t value) {
    this->b = value >> 8;
    this->c = (uint8_t)value;
}

uint16_t Z80::getBC() {
    return combineRegisters(this->b, this->c);
}

void Z80::setDE(uint16_t value) {
    this->d = value >> 8;
    this->e = (uint8_t)value;
}

uint16_t Z80::getDE() {
    return combineRegisters(this->d, this->e);
}

void Z80::setHL(uint16_t value) {
    this->h = value >> 8;
    this->l = (uint8_t) value;
}

uint16_t Z80::getHL() {
    return combineRegisters(this->h, this->l);
}

void Z80::setAF(uint16_t value) {
    this->a = value >> 8;
    this->f = (uint8_t)value;
}

uint16_t Z80::getAF() {
    return combineRegisters(this->a, this->f);
}

void Z80::setBCP(uint16_t value) {
    this->bp = value >> 8;
    this->cp = (uint8_t)value;
}

uint16_t Z80::getBCP() {
    return combineRegisters(this->bp, this->cp);
}

void Z80::setDEP(uint16_t value) {
    this->dp = value >> 8;
    this->ep = (uint8_t)value;
}

uint16_t Z80::getDEP() {
    return combineRegisters(this->dp, this->ep);
}

void Z80::setHLP(uint16_t value) {
    this->hp = value >> 8;
    this->lp = (uint8_t) value;
}

uint16_t Z80::getHLP() {
    return combineRegisters(this->hp, this->lp);
}

void Z80::setAFP(uint16_t value) {
    this->ap = value >> 8;
    this->fp = (uint8_t)value;
}

uint16_t Z80::getAFP() {
    return combineRegisters(this->ap, this->fp);
}
