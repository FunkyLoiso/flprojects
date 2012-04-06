#-------------------------------------------------
#
# Project created by QtCreator 2012-03-27T00:19:10
#
#-------------------------------------------------

QT       += core, network , xml

QT       -= gui

TARGET = DarkMessCli
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    tcpclient.cpp

HEADERS += \
    tcpclient.h
