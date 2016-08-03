#-------------------------------------------------
#
# Project created by QtCreator 2016-07-15T11:07:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SampleCreator
TEMPLATE = app

VERSION = 1.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    qclicklabel.cpp \
    qflipthread.cpp \
    qsizewidget.cpp

HEADERS  += mainwindow.h \
    qclicklabel.h \
    qflipthread.h \
    qsizewidget.h

FORMS    += mainwindow.ui \
    qsizewidget.ui


RESOURCES += \
    ico.qrc

RC_FILE += appico.rc
