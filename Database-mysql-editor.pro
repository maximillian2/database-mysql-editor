#-------------------------------------------------
#
# Project created by QtCreator 2014-03-21T09:29:28
#
#-------------------------------------------------

QT       += core gui sql

TARGET = Database-mysql-editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    addtable.cpp \
    altertable.cpp

HEADERS  += mainwindow.h \
    addtable.h \
    altertable.h

FORMS    += mainwindow.ui \
    addtable.ui \
    altertable.ui

RESOURCES += \
    images.qrc
