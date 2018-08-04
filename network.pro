#-------------------------------------------------
#
# Project created by QtCreator 2017-02-05T11:01:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = network
TEMPLATE = app
CONFIG += qt warn_on thread exceptions
DEFINES += NETWORK_DLL
DESTDIR = ..\lib


INCLUDEPATH += \
              ../include \
#              ../include/boost
#LIBPATH += \
#              D:\qt-workspace\lib\lib64-msvc-12.0 \

SOURCES += main.cpp\
    #hprotocol.cpp \
    #hmessage.cpp \
    hmsg.cpp \
    hnetthread.cpp \
    hnetmonitor.cpp \
    hmyhighligher.cpp \
    hnetglobal.cpp \
    hnetworkapp.cpp \
    hconnect.cpp \
    htcpserver.cpp
    #hnethandle.cpp

HEADERS  += networkapi.h \
    #hprotocol.h \
    #hmessage.h \
    hmsg.h \
    hnetthread.h \
    hnetmonitor.h \
    hmyhighligher.h \
    hnetglobal.h \
    hnetworkapp.h \
    hconnect.h \
    htcpserver.h
    #hnethandle.h

RESOURCES += \
    #network.qrc

LIBS +=  -L../lib/lib64-boost-vc12
win32{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj
    #LIBS += \
         #../lib/lib64-msvc-12.0/libboost_system-vc120-mt-gd-1_60.lib \
         #../lib/lib64-msvc-12.0/libboost_date_time-vc120-mt-gd-1_60.lib \
         #../lib/lib64-msvc-12.0/libboost_regex-vc120-mt-gd-1_60.lib \
         #../lib/lib64-msvc-12.0/libboost_atomic-vc120-mt-gd-1_60.lib \
         #../lib/lib64-msvc-12.0/libboost_thread-vc120-mt-gd-1_60.lib \
         #../lib/lib64-msvc-12.0/libboost_chrono-vc120-mt-gd-1_60.lib \
}

unix {
        UI_DIR	=	.ui
        MOC_DIR	=	.moc
        OBJECTS_DIR	=	.obj
}

FORMS += \
    UI/netmonitor.ui

