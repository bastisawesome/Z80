#include "mainwindow.h"

//#include <QApplication>
#include <memory>

#include "z80.h"

/*
 * TODO:
 * Document Z80.cpp
 * Implement instructions
 * Assign flag masks outside of constructor
 * FIX ADDHL, IT'S A BROKEN MESS RIGHT NOW (FLAGS ARE SHIT)
 */

int main(int argc, char *argv[]) {
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/

    // For debug ROM, 0x00-0x3F are operations.
    // Beyond that is data. HALT MUST be kept at 0x3F to prevent
    // the CPU from reading data as opcodes.
    std::array<uint8_t, 0x7F> rom = {
        0xFF,               // NOP              | 0x00
        0x00,               // NOP              | 0x01
        0x01,               // LD BC            | 0x02
        0x40,               // Lower 0x40 (64)  | 0x03
        0x00,               // Upper 0x40 (64)  | 0x04
        0x02,               // LD (BC), A       | 0x05
        0x03,               // INC BC           | 0x06
        0x04,               // INC B            | 0x07
        0x05,               // DEC B            | 0x08
        0x06,               // LD B, n          | 0x09
        0x01,               // 0x01 (1)         | 0x0A
        0x07,               // RLCA             | 0x0B
        0x08,               // EX AF, AF'       | 0x0C
        0x09,               // ADD HL, BC       | 0x0D
        0x01,               // LD BC            | 0x0E
        0x42,               // Lower 0x42 (66)  | 0x0F
        0x00,               // Upper 0x42 (66)  | 0x10
        0x0A,               // LD A, (BC)       | 0x11
        0x0B,               // DEC BC           | 0x12
        0x0C,               // INC C            | 0x13
        0x0D,               // DEC C            | 0x14
        0x0E,               // LD C, n          | 0x15
        0x13,               // 0x13 (19)        | 0x16
        0x0F,               // RRCA             | 0x17
        0x00,               // NOP              | 0x18
        0x00,               // NOP              | 0x19
        0x00,               // NOP              | 0x1A
        0x00,               // NOP              | 0x1B
        0x00,               // NOP              | 0x1C
        0x00,               // NOP              | 0x1D
        0x00,               // NOP              | 0x1E
        0x00,               // NOP              | 0x1F
        0x00,               // NOP              | 0x20
        0x00,               // NOP              | 0x21
        0x00,               // NOP              | 0x22
        0x00,               // NOP              | 0x23
        0x00,               // NOP              | 0x24
        0x00,               // NOP              | 0x25
        0x00,               // NOP              | 0x26
        0x00,               // NOP              | 0x27
        0x00,               // NOP              | 0x28
        0x00,               // NOP              | 0x29
        0x00,               // NOP              | 0x2A
        0x00,               // NOP              | 0x2B
        0x00,               // NOP              | 0x2C
        0x00,               // NOP              | 0x2D
        0x00,               // NOP              | 0x2E
        0x00,               // NOP              | 0x2F
        0x00,               // NOP              | 0x30
        0x00,               // NOP              | 0x31
        0x00,               // NOP              | 0x32
        0x00,               // NOP              | 0x33
        0x00,               // NOP              | 0x34
        0x00,               // NOP              | 0x35
        0x00,               // NOP              | 0x36
        0x00,               // NOP              | 0x37
        0x00,               // NOP              | 0x38
        0x00,               // NOP              | 0x39
        0x00,               // NOP              | 0x3A
        0x00,               // NOP              | 0x3B
        0x00,               // NOP              | 0x3C
        0x00,               // NOP              | 0x3D
        0x00,               // NOP              | 0x3E
        0x76,               // HALT             | 0x3F
        0x00,               // NOP              | 0x40
        0x00,               // NOP              | 0x41
        0xFF,               // 0xFF (255)       | 0x42
        0x00,               // NOP              | 0x43
        0x00,               // NOP              | 0x44
        0x00,               // NOP              | 0x45
        0x00,               // NOP              | 0x46
        0x00,               // NOP              | 0x47
        0x00,               // NOP              | 0x48
        0x00,               // NOP              | 0x49
        0x00,               // NOP              | 0x4A
        0x00,               // NOP              | 0x4B
        0x00,               // NOP              | 0x4C
        0x00,               // NOP              | 0x4D
        0x00,               // NOP              | 0x4E
        0x00,               // NOP              | 0x4F
        0x00,               // NOP              | 0x50
        0x00,               // NOP              | 0x51
        0x00,               // NOP              | 0x52
        0x00,               // NOP              | 0x53
        0x00,               // NOP              | 0x54
        0x00,               // NOP              | 0x55
        0x00,               // NOP              | 0x56
        0x00,               // NOP              | 0x57
        0x00,               // NOP              | 0x58
        0x00,               // NOP              | 0x59
        0x00,               // NOP              | 0x5A
        0x00,               // NOP              | 0x5B
        0x00,               // NOP              | 0x5C
        0x00,               // NOP              | 0x5D
        0x00,               // NOP              | 0x5E
        0x00,               // NOP              | 0x5F
        0x00,               // NOP              | 0x60
        0x00,               // NOP              | 0x61
        0x00,               // NOP              | 0x62
        0x00,               // NOP              | 0x63
        0x00,               // NOP              | 0x64
        0x00,               // NOP              | 0x65
        0x00,               // NOP              | 0x66
        0x00,               // NOP              | 0x67
        0x00,               // NOP              | 0x68
        0x00,               // NOP              | 0x69
        0x00,               // NOP              | 0x6A
        0x00,               // NOP              | 0x6B
        0x00,               // NOP              | 0x6C
        0x00,               // NOP              | 0x6D
        0x00,               // NOP              | 0x6E
        0x00,               // NOP              | 0x6F
        0x00,               // NOP              | 0x70
        0x00,               // NOP              | 0x71
        0x00,               // NOP              | 0x72
        0x00,               // NOP              | 0x73
        0x00,               // NOP              | 0x74
        0x00,               // NOP              | 0x75
        0x00,               // NOP              | 0x76
        0x00,               // NOP              | 0x77
        0x00,               // NOP              | 0x78
        0x00,               // NOP              | 0x79
        0x00,               // NOP              | 0x7A
        0x00,               // NOP              | 0x7B
        0x00,               // NOP              | 0x7C
        0x00,               // NOP              | 0x7D
        0x00,               // NOP              | 0x7E
    };

    std::unique_ptr<Z80> z80(new Z80(rom));

    z80->run();

    return 0;
}
