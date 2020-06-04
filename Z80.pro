QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 console

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    z80/Z80.cpp \
    z80/Z80Cpu.cpp \
    z80/Z80Mem.cpp

HEADERS += \
    generics/helpers.h \
    generics/ops.h \
    generics/types.h \
    mainwindow.h \
    z80/Z80.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += \
    ./z80 \
    ./generics

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .astylerc \
    .gitignore \
    Z80.cflags \
    Z80.cxxflags \
    old/main/java/com/bastisawesome/controllers/MemoryWindowController.java \
    old/main/java/com/bastisawesome/generics/Memory.java \
    old/main/java/com/bastisawesome/run/Main.java \
    old/main/java/com/bastisawesome/views/MainWindow.java \
    old/main/java/com/bastisawesome/views/MemoryWindow.java \
    old/main/java/com/bastisawesome/z80/Z80Cpu.java \
    old/main/java/com/bastisawesome/z80/Z80Device.java \
    old/main/java/com/bastisawesome/z80/Z80Memory.java

# Custom definitions
Debug: DEFINES += Z80_DEBUG
