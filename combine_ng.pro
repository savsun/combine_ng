#-------------------------------------------------
#
# Project created by QtCreator 2014-09-22T12:21:23
#
#-------------------------------------------------

QT       += core gui opengl xml
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = combine_ng
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qmapview.cpp \
    texture.cpp \
    model.cpp \
    parser.cpp \
    kohonen.cpp \
    kmeans.cpp

HEADERS  += mainwindow.h \
    qmapview.h \
    texture.h \
    all.h \
    model.h \
    parser.h \
    kohonen.h \
    kmeans.h

INCLUDEPATH += "/usr/include/GL"
FORMS    += mainwindow.ui \
    model.ui
LIBS+= -lgdal -lopencv_core -lopencv_imgproc -lopencv_highgui
LIBS += -lGLU
LIBS += -lfmll_lib -lfmll_math_graph -lfmll_math_matrix -lfmll_math_random
LIBS += -lfmll_math_various -lfmll_ann_base -lfmll_ann_ff -lfmll_ann_mlp -lfmll_ann_som -lfmll_ann_pca -lfmll_ann_svm
