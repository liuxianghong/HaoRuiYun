#-------------------------------------------------
#
# Project created by QtCreator 2016-12-12T11:03:19
#
#-------------------------------------------------

QT       += core gui network

mac::QMAKE_MAC_SDK = macosx10.12
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nettest
TEMPLATE = app
CONFIG *= c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    KUploadFile.cpp \
    NetworkManager.cpp \
    HttpRequest.cpp \
    FileManager.cpp

HEADERS  += mainwindow.h \
    KUploadFile.h \
    NetworkManager.h \
    HttpRequest.h \
    FileManager.h

FORMS    += mainwindow.ui
