#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T13:59:08
#
#-------------------------------------------------

QT       += core gui webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CQS
TEMPLATE = app

CONFIG += c++11

LIBS += -lpthread libwsock32 libws2_32

SOURCES += main.cpp\
        widget.cpp \
    xml/src/tinystr.cpp \
    xml/src/tinyxml.cpp \
    xml/src/tinyxmlerror.cpp \
    xml/src/tinyxmlparser.cpp \
    sendthread.cpp \
    netclient.cpp \
    task/BrandsTask.cpp \
    task/CarListTask.cpp \
    task/CarPicTask.cpp \
    factory/TaskFactory.cpp \
    ReceThread.cpp \
    task/PicLinkTask.cpp

INCLUDEPATH += xml/src xml/inc task factory



HEADERS  += widget.h \
    xml/inc/tinystr.h \
    xml/inc/tinyxml.h \
    sendthread.h \
    netclient.h \
    task/ITask.h \
    task/BrandsTask.h \
    task/CarListTask.h \
    task/CarPicTask.h \
    factory/IFactory.h \
    factory/TaskFactory.h \
    ReceThread.h \
    Define.h \
    task/PicLinkTask.h


FORMS    += widget.ui
