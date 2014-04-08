#-------------------------------------------------
#
# Project created by QtCreator 2014-04-09T00:25:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTNodes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    terminalwidget.cpp \
    dojonode.cpp \
    dojonetwork.cpp \
    clientarea.cpp

HEADERS  += mainwindow.h \
    terminalwidget.h \
    dojonode.h \
    dojonetwork.h \
    clientarea.h

FORMS    += mainwindow.ui
