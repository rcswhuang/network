#-------------------------------------------------
#
# Project created by QtCreator 2017-02-05T11:01:21
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = network
TEMPLATE = app
CONFIG += qt warn_on thread exceptions
DEFINES += NETWORK_DLL


INCLUDEPATH += \
              ../include

SOURCES += main.cpp\
    hprotocol.cpp \
    hmsg.cpp \
    hnetthread.cpp \
    hnetmonitor.cpp \
    hmyhighligher.cpp \
    hnetglobal.cpp \
    htcpserver.cpp \
    hnetmanager.cpp \
    htcpconnect.cpp \
    hnethandle.cpp

HEADERS  += \
    hprotocol.h \
    hmsg.h \
    hnetthread.h \
    hnetmonitor.h \
    hmyhighligher.h \
    #hnetglobal.h \
    htcpserver.h \
    hnetmanager.h \
    htcpconnect.h \
    hnetworkdef.h \
    hnethandle.h

RESOURCES += \
    #network.qrc

#LIBS +=  -L../lib -lkernel

win32{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj
    LIBS +=  -L../lib/lib64-boost-vc12
}

unix{
        #UI_DIR	= temp/ui
        #MOC_DIR	= temp/moc
        #OBJECTS_DIR = temp/obj
      LIBS += -L../lib/lib64-boost-linux \
            -lboost_system-mt \
            -lboost_date_time-mt \
            -lboost_regex-mt \
            -lboost_atomic-mt \
            -lboost_thread-mt \
            -lboost_chrono-mt
}

FORMS += \
    UI/netmonitor.ui

