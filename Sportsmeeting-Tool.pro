QT       += core gui

RC_ICONS += main.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sportsmeeting-Tool
TEMPLATE = app


SOURCES += main.cpp\
    iconhelper.cpp \
    frmmessagebox.cpp \
    cJSON.cpp \
    mJSON.cpp \
    systemmain.cpp \
    adminmain.cpp \
    atheletmain.cpp

HEADERS  += iconhelper.h \
    frmmessagebox.h \
    myhelper.h \
    cJSON.h \
    mJSON.h \
    systemmain.h \
    adminmain.h \
    atheletmain.h

FORMS    += frmmessagebox.ui \
    systemmain.ui \
    adminmain.ui \
    atheletmain.ui

MOC_DIR=temp/moc
RCC_DIR=temp/rcc
UI_DIR=temp/ui
OBJECTS_DIR=temp/obj
DESTDIR=bin

win32:RC_FILE=main.rc

RESOURCES += \
    rc.qrc

DISTFILES +=
