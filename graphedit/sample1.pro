#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T16:01:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sample1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphedit.cpp \
    graph.cpp \
    io/graphwriter.cpp \
    io/graphreader.cpp \
    graphbackground.cpp \
	algorithm/fordfulkerson.cpp \
    algorithm/prim.cpp \
	algorithm/dijkstra.cpp \
    about_dev.cpp \
    manual.cpp \
    algorithm/kruskal.cpp

HEADERS  += mainwindow.h \
    graphedit.h \
    graph.h \
    io/graphwriter.h \
    io/graphreader.h \
    graphbackground.h \
	algorithm/dijkstra.h \
	algorithm/fordfulkerson.h \
    algorithm/algorithm.h \
    algorithm/prim.h \
    about_dev.h \
    manual.h \
    algorithm/kruskal.h

FORMS    += mainwindow.ui \
    about_dev.ui \
    manual.ui

RESOURCES += \
    img.qrc
