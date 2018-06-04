#-------------------------------------------------
#
# Project created by QtCreator 2018-05-12T17:37:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pic2wp
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    daye.cpp \
    downloader.cpp \
    httpupload.cpp \
    wpcontent.cpp \
    smms.cpp \
    tuchuang.cpp \
    proxyform.cpp \
    s6tu.cpp \
    tuchuangloginform.cpp \
    pic303.cpp \
    picsource.cpp \
    t66y.cpp \
    picdownloader.cpp \
    meitulu.cpp \
    s2tu.cpp

HEADERS += \
        mainwindow.h \
    daye.h \
    downloader.h \
    httpupload.h \
    wpcontent.h \
    smms.h \
    tuchuang.h \
    proxyform.h \
    s6tu.h \
    tuchuangloginform.h \
    pic303.h \
    picsource.h \
    t66y.h \
    picdownloader.h \
    meitulu.h \
    s2tu.h

FORMS += \
        mainwindow.ui \
    proxyform.ui \
    tuchuangloginform.ui
