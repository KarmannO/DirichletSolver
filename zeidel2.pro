#-------------------------------------------------
#
# Project created by QtCreator 2016-04-07T22:58:00
#
#-------------------------------------------------

QT       += core widgets opengl

QT       -= gui

TARGET = zeidel2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    matrixtoperation.cpp \
    widget.cpp \
    camera.cpp \
    controlbuttonstate.cpp \
    followcamera.cpp

HEADERS += \
    matrixtoperation.h \
    widget.h \
    camera.h \
    controlbuttonstate.h \
    followcamera.h

LIBS += -lopengl32 -lglu32
