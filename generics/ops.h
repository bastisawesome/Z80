#ifndef OPS_H
#define OPS_H

#include <array>
#include <string>

// Array of opcode names for debug purposes
#ifdef Z80_DEBUG
std::array<std::string, 256> opNames = {
    "NOP",              // 0x00
    "LD BC, **",        // 0x01
    "LD (BC), A",       // 0x02
    "INC BC",           // 0x03
    "INC B",            // 0x04
    "DEC B",            // 0x05
    "LD B, *",          // 0x06
    "RLCA",             // 0x07
    "EX AF, AF'",       // 0x08
    "ADD HL, BC",       // 0x09
    "LD A, (BC)",       // 0x0A
    "DEC BC",           // 0x0B
    "INC C",            // 0x0C
    "DEC C",            // 0x0D
    "LD C, *",          // 0x0E
    "RRCA",             // 0x0F
    "DJNZ *",           // 0x10
    "LD DE, **",        // 0x11
    "LD (DE), A",       // 0x12
    "INC DE",           // 0x13
    "INC D",            // 0x14
    "DEC D",            // 0x15
    "LD D, *",          // 0x16
    "RLA",              // 0x17
    "JR *",             // 0x18
    "ADD HL, DE",       // 0x19
    "LD A, (DE)",       // 0x1A
    "DEC DE",           // 0x1B
    "INC E",            // 0x1C
    "DEC E",            // 0x1D
    "LD E, *",          // 0x1E
    "RRA",              // 0x1F
    "JR NZ, *",         // 0x20
    "LD HL, **",        // 0x21
    "LD (**), HL",      // 0x22
    "INC HL",           // 0x23
    "INC H",            // 0x24
    "DEC H",            // 0x25
    "LD H, *",          // 0x26
    "DAA",              // 0x27
    "JR Z, *",          // 0x28
    "ADD HL, HL",       // 0x29
    "LD HL, (**)",      // 0x2A
    "DEC HL",           // 0x2B
    "INC L",            // 0x2C
    "DEC L",            // 0x2D
    "LD L, *",          // 0x2E
    "CPL",              // 0x2F
    "JR NC, *",         // 0x30
    "LD SP, **",        // 0x31
    "LD (**), A",       // 0x32
    "INC SP",           // 0x33
    "INC (HL)",         // 0x34
    "DEC (HL)",         // 0x35
    "LD (HL), *",       // 0x36
    "SCF",              // 0x37
    "JR C, *",          // 0x38
    "ADD HL, SP",       // 0x39
    "LD A, (**)",       // 0x3A
    "DEC SP",           // 0x3B
    "INC A",            // 0x3C
    "DEC A",            // 0x3D
    "LD A, *",          // 0x3E
    "CCF",              // 0x3F
    "LD B, B",          // 0x40
    "LD B, C",          // 0x41
    "LD B, D",          // 0x42
    "LD B, E",          // 0x43
    "LD B, H",          // 0x44
    "LD B, L",          // 0x45
    "LD B, (HL)",       // 0x46
    "LD B, A",          // 0x47
    "LD C, B",          // 0x48
    "LD C, C",          // 0x49
    "LD C, D",          // 0x4A
    "LD C, E",          // 0x4B
    "LD C, H",          // 0x4C
    "LD C, L",          // 0x4D
    "LD C, (HL)",       // 0x4E
    "LD C, A",          // 0x4F
    "LD D, B",          // 0x50
    "LD D, C",          // 0x51
    "LD D, D",          // 0x52
    "LD D, E",          // 0x53
    "LD D, H",          // 0x54
    "LD D, L",          // 0x55
    "LD D, (HL)",       // 0x56
    "LD D, A",          // 0x57
    "LD E, B",          // 0x58
    "LD E, C",          // 0x59
    "LD E, D",          // 0x5A
    "LD E, E",          // 0x5B
    "LD E, H",          // 0x5C
    "LD E, L",          // 0x5D
    "LD E, (HL)",       // 0x5E
    "LD E, A",          // 0x5F
    "LD H, B",          // 0x60
    "LD H, C",          // 0x61
    "LD H, D",          // 0x62
    "LD H, E",          // 0x63
    "LD H, H",          // 0x64
    "LD H, L",          // 0x65
    "LD H, (HL)",       // 0x66
    "LD H, A",          // 0x67
    "LD L, B",          // 0x68
    "LD L, C",          // 0x69
    "LD L, D",          // 0x6A
    "LD L, E",          // 0x6B
    "LD L, H",          // 0x6C
    "LD L, L",          // 0x6D
    "LD L, (HL)",       // 0x6E
    "LD L, A",          // 0x6F
    "LD (HL), B",       // 0x70
    "LD (HL), C",       // 0x71
    "LD (HL), D",       // 0x72
    "LD (HL), E",       // 0x73
    "LD (HL), H",       // 0x74
    "LD (HL), L",       // 0x75
    "HALT",             // 0x76
    "LD (HL), A",       // 0x77
    "LD A, B",          // 0x78
    "LD A, C",          // 0x79
    "LD A, D",          // 0x7A
    "LD A, E",          // 0x7B
    "LD A, H",          // 0x7C
    "LD A, L",          // 0x7D
    "LD A, (HL)",       // 0x7E
    "LD A, A",          // 0x7F
    "ADD A, B",
    "ADD A, C",
    "ADD A, D",
    "ADD A, E",
    "ADD A, H",
    "ADD A, L",
    "ADD A, (HL)",
    "ADD A, A",
    "ADC A, B",
    "ADC A, C",
    "ADC A, D",
    "ADC A, E",
    "ADC A, H",
    "ADC A, L",
    "ADC A, (HL)",
    "ADC A, A",
    "SUB B",
    "SUB C",
    "SUB D",
    "SUB E",
    "SUB H",
    "SUB L",
    "SUB (HL)",
    "SUB A",
    "SBC A, B",
    "SBC A, C",
    "SBC A, D",
    "SBC A, E",
    "SBC A, H",
    "SBC A, L",
    "SBC A, (HL)",
    "SBC A, A",
    "AND B",
    "AND C",
    "AND D",
    "AND E",
    "AND H",
    "AND L",
    "AND (HL)",
    "AND A",
    "XOR B",
    "XOR C",
    "XOR D",
    "XOR E",
    "XOR H",
    "XOR L",
    "XOR (HL)",
    "XOR A",
    "OR B",
    "OR C",
    "OR D",
    "OR E",
    "OR H",
    "OR L",
    "OR (HL)",
    "OR A",
    "CP B",
    "CP C",
    "CP D",
    "CP E",
    "CP H",
    "CP L",
    "CP (HL)",
    "CP A",
    "RET NZ",
    "POP BC",
    "JP NZ, **",
    "JP **",
    "CALL NZ, **",
    "PUSH BC",
    "ADD A, *",
    "RST 00h",
    "RET Z",
    "RET",
    "JP Z, **",
    "BITS (THIS IS NOT AN INSTRUCTION!!!!!)",
    "CALL Z, **",
    "CALL **",
    "ADC A, *",
    "RST 08h",
    "RET NC",
    "POP DE",
    "JP NC, **",
    "OUT (*), A",
    "CALL NC, **",
    "PUSH DE",
    "SUB *",
    "RST 10h",
    "RET C",
    "EXX",
    "JP C, **",
    "IN A, (*)",
    "CALL C, **",
    "IX (THIS IS NOT AN INSTRUCTION!!!!!)",
    "SBC A, *",
    "RST 18h",
    "RET PO",
    "POP HL",
    "JP PO, **",
    "EX (SP), HL",
    "CALL PO, **",
    "PUSH HL",
    "AND *",
    "RST 20h",
    "RET PE",
    "JP (HL)",
    "JP PE, **",
    "EX DE, HL",
    "CALL PE, **",
    "EXTD (THIS IS NOT AN INSTRUCTION!!!!!)",
    "XOR *",
    "RST 28h",
    "RET P",
    "POP AF",
    "JP P, **",
    "DI",
    "CALL P, **",
    "PUSH AF",
    "OR *",
    "RST 30h",
    "RET M",
    "LD SP, HL",
    "JP M, **",
    "CALL M, **",
    "IY (THIS IS NOT AN INSTRUCTION!!!!!)",
    "CP, *",
    "RST 38h"
};
#endif

#endif // OPS_H
