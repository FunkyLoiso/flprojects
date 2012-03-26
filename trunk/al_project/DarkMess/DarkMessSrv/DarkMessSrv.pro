#-------------------------------------------------
#
# Project created by QtCreator 2012-03-27T00:03:22
#
#-------------------------------------------------

QT       += core, network

QT       -= gui

TARGET = DarkMessSrv
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcpserver.cpp

HEADERS += \
    tcpserver.h
