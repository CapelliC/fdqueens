#-------------------------------------------------
#
# Project created by QtCreator 2013-04-26T13:39:29
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fdqueens
TEMPLATE = app

# please, not obsolete compiler
QMAKE_CXXFLAGS += -std=c++0x

FORMS += \
    dialog.ui

DEFINES += PL_SAFE_ARG_MACROS

windows {
    SwiPl = "C:\Program Files\pl"
    INCLUDEPATH += $$SwiPl\include
    LIBS += -L$$SwiPl\bin -lswipl
}
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += swipl
}

OTHER_FILES += \
    fdqueens.pl \
    README.md

RESOURCES += \
    fdqueens.qrc

# avoid full build of loqt components for this test
# pqConsole component source included
# lqUty component source included

DEFINES += PQCONSOLE_STATIC
DEFINES += LQUTY_STATIC

windows {
    pqConsole = $$PWD/../loqt-master/pqConsole
}
unix {
    pqConsole = $$PWD/../pqConsole
}
INCLUDEPATH += $$pqConsole
DEPENDPATH += $$pqConsole

windows {
    lqUty = $$PWD/../loqt-master/lqUty
}
unix {
    lqUty = $$PWD/../lqUty
}
INCLUDEPATH += $$lqUty
DEPENDPATH += $$lqUty

SOURCES += \
    main.cpp \
    dialog.cpp \
    $$pqConsole/pqConsole.cpp \
    $$pqConsole/pqMainWindow.cpp \
    $$pqConsole/pqMiniSyntax.cpp \
    $$pqConsole/SwiPrologEngine.cpp \
    $$pqConsole/Swipl_IO.cpp \
    $$pqConsole/Completion.cpp \
    $$pqConsole/ConsoleEdit.cpp \
    $$pqConsole/reflexive.cpp \
    $$pqConsole/FlushOutputEvents.cpp \
    $$pqConsole/Preferences.cpp \
    $$lqUty/lqUty.cpp \
    $$lqUty/ParenMatching.cpp

HEADERS += \
    dialog.h \
    $$pqConsole/pqConsole.h \
    $$pqConsole/pqMainWindow.h \
    $$pqConsole/pqMiniSyntax.h \
    $$pqConsole/SwiPrologEngine.h \
    $$pqConsole/Swipl_IO.h \
    $$pqConsole/Completion.h \
    $$pqConsole/ConsoleEdit.h \
    $$pqConsole/reflexive.h \
    $$pqConsole/FlushOutputEvents.h \
    $$pqConsole/Preferences.h \
    $$lqUty/lqUty.h \
    $$lqUty/ParenMatching.h
