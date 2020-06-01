#include "mainwindow.h"

//#include <QApplication>
#include <memory>

#include "z80.h"

/*
 * TODO:
 * Document Z80.cpp
 * Implement instructions
 * Implement debug functions
 * Implement run
 * Store opcode names for debug output
 */

int main(int argc, char *argv[]) {
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/

    std::array<uint8_t, 10> rom = {
        0x01,               // LD BC
        0x00,               // Upper 0xB
        0xB,                // Lower 0xB
        0x02,               // LD (BC), A
        0x76                // HALT
    };

    std::unique_ptr<Z80> z80(new Z80(rom));

    z80->run();

    return 0;
}
