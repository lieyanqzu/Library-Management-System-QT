#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T23:14:21
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Library
TEMPLATE = app


SOURCES += main.cpp\
    database.cpp \
    logindialog.cpp \
    admin.cpp \
    reader.cpp \
    book.cpp \
    booklent.cpp \
    mainwindow.cpp \
    setting.cpp \
    operations.cpp

HEADERS  += \
    database.h \
    datamodel.h \
    logindialog.h \
    admin.h \
    reader.h \
    book.h \
    booklent.h \
    mainwindow.h \
    setting.h \
    operations.h

FORMS += \
    logindialog.ui \
    mainwindow.ui

DISTFILES +=

RESOURCES += \
    icon.qrc
