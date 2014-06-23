#-------------------------------------------------
#
# Project created by QtCreator 2014-04-09T00:25:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QTNodes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../qcustomplot/qcustomplot.cpp \
    dojo/dojosynapse.cpp \
    dojo/dojonode.cpp \
    dojo/dojonetwork.cpp \
    graph/node.cpp \
    graph/graphwidget.cpp \
    graph/edge.cpp \
    docks/watchwidget.cpp \
    docks/terminalwidget.cpp \
    docks/clientarea.cpp

HEADERS  += mainwindow.h \
    ../qcustomplot/qcustomplot.h \
    dojo/dojosynapse.h \
    dojo/dojonode.h \
    dojo/dojonetwork.h \
    graph/node.h \
    graph/graphwidget.h \
    graph/edge.h \
    docks/watchwidget.h \
    docks/terminalwidget.h \
    docks/clientarea.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../build-QTNodes-Desktop_Qt_5_3_0_MinGW_32bit-Debug/network_1.dojo
