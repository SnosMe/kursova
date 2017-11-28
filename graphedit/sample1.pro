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

HEADERS  += mainwindow.h \
    graphedit.h \
    graph.h \
    io/graphwriter.h \
    io/graphreader.h \
    graphbackground.h \
	algorithm/dijkstra.h \
	algorithm/fordfulkerson.h \

FORMS    += mainwindow.ui

RESOURCES += \
    img.qrc
