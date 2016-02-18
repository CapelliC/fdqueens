#--------------------------------------------------
# fdqueens.pro: project file
#  visualizing SWI-Prolog attributed variables in Qt
#  N-queens solved with CLP(FD)
#--------------------------------------------------
# Author        : Carlo Capelli
# Copyright (C): 2013,2014,2015,2016

QT += core gui widgets

TARGET = fdqueens
TEMPLATE = app

CONFIG += C++11

SOURCES += \
    main.cpp \
    dialog.cpp

HEADERS += \
    dialog.h \
    engine_hsm.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../pqConsole/release/ -lpqConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../pqConsole/debug/ -lpqConsole
else:unix:!symbian: LIBS += -L$$OUT_PWD/../pqConsole/ -lpqConsole

INCLUDEPATH += $$PWD/../pqConsole
DEPENDPATH += $$PWD/../pqConsole

windows {
    lqUty = $$PWD/../loqt-master/lqUty
}
unix {
    lqUty = $$PWD/../lqUty
}
INCLUDEPATH += $$lqUty
DEPENDPATH += $$lqUty

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lqUty/release/ -llqUty
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lqUty/debug/ -llqUty
else:unix:!symbian: LIBS += -L$$OUT_PWD/../lqUty/ -llqUty

INCLUDEPATH += $$PWD/../lqUty
DEPENDPATH += $$PWD/../lqUty
