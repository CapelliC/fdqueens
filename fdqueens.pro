#-------------------------------------------------
#
# Project created by QtCreator 2013-04-26T13:39:29
#
#-------------------------------------------------

QT += core gui

TARGET = fdqueens
TEMPLATE = app


SOURCES += \
    main.cpp \
    dialog.cpp

HEADERS += \
    dialog.h

FORMS += \
    dialog.ui

CONFIG += link_pkgconfig
PKGCONFIG += swipl

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
