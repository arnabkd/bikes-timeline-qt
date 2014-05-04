#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T14:13:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bikeracks
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/bikerack.cpp \
    src/bikeracksystem.cpp \
    src/rackstatus.cpp \
    src/mathutils.cpp \
    src/datasetloader.cpp

HEADERS  += src/mainwindow.h \
    src/bikerack.h \
    src/bikeracksystem.h \
    src/rackstatus.h \
    src/mathutils.h \
    src/datasetloader.h

