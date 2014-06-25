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

SOURCES += \
    main.cpp \
    dialog.cpp

HEADERS += \
    dialog.h

FORMS += \
    dialog.ui

windows {
    SwiPl = "C:\Program Files\pl"
    INCLUDEPATH += $$SwiPl\include
    LIBS += -L$$SwiPl\bin -lswipl
}
unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += swipl
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../pqConsole/release/ -lpqConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../pqConsole/debug/ -lpqConsole
else:symbian: LIBS += -lpqConsole
else:unix: LIBS += -L$$OUT_PWD/../pqConsole/ -lpqConsole

INCLUDEPATH += $$PWD/../pqConsole
DEPENDPATH += $$PWD/../pqConsole

OTHER_FILES += \
    fdqueens.pl \
    README.md

RESOURCES += \
    fdqueens.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../lqUty/release/ -llqUty
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../lqUty/debug/ -llqUty
else:unix: LIBS += -L$$OUT_PWD/../lqUty/ -llqUty

INCLUDEPATH += $$PWD/../lqUty
DEPENDPATH += $$PWD/../lqUty
