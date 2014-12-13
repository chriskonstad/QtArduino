#-------------------------------------------------
#
# Project created by QtCreator 2012-07-25T22:14:49
#
#-------------------------------------------------


#Must add the "network" tag to include the UDP classes
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtArduino
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0
