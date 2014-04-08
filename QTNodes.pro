#-------------------------------------------------
#
# Project created by QtCreator 2014-03-20T09:56:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTNodes
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    terminalwidget.cpp \
    dojonetwork.cpp \
    dojonode.cpp \
    clientarea.cpp

HEADERS  += mainwindow.h \
    terminalwidget.h \
    dojonetwork.h \
    dojonode.h \
    clientarea.h

FORMS    += mainwindow.ui
