# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = Lonpos_F_L
DESTDIR = ../Win32/Debug
QT += core gui
CONFIG += debug
DEFINES += QT_LARGEFILE_SUPPORT QT_DLL
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += ./lonpos_f_l.h \
    ./FieldWidget.h \
    ./Figure.h \
    ./FieldController.h
SOURCES += ./main.cpp \
    ./lonpos_f_l.cpp \
    ./FieldWidget.cpp \
    ./Figure.cpp \
    ./FieldController.cpp
FORMS += ./lonpos_f_l.ui
RESOURCES += ./lonpos_f_l.qrc
