#include "mainwindow.h"

#include <QApplication>
#include <memory>

#include "z80.h"

/*
 * TODO:
 * Document Z80.cpp
 * Implement instructions
 * Implement debug functions
 * Implement run
 */

int main(int argc, char *argv[]) {
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();*/

    std::array<uint8_t, 10> rom = {
    };

    std::unique_ptr<Z80> z80(new Z80(rom));

    z80->run();

    return 0;
}
