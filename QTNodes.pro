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
    terminalwidget.cpp \    
    clientarea.cpp \
    watchwidget.cpp \
    ../qcustomplot/qcustomplot.cpp \    
    dojo/dojosynapse.cpp \
    dojo/dojonode.cpp \
    dojo/dojonetwork.cpp

HEADERS  += mainwindow.h \
    terminalwidget.h \   
    clientarea.h \
    watchwidget.h \
    ../qcustomplot/qcustomplot.h \
    dojo/dojosynapse.h \
    dojo/dojonode.h \
    dojo/dojonetwork.h

FORMS    += mainwindow.ui
