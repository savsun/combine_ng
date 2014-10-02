#-------------------------------------------------
#
# Project created by QtCreator 2014-09-22T12:21:23
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = combine_ng
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qmapview.cpp \
    texture.cpp

HEADERS  += mainwindow.h \
    qmapview.h \
    texture.h \
    all.h

FORMS    += mainwindow.ui
LIBS+= -lgdal -lopencv_core -lopencv_imgproc -lopencv_highgui
