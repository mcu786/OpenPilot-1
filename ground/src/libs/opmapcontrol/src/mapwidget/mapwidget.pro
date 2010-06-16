TEMPLATE = lib
TARGET = opmapwidget
DEFINES += OPMAPWIDGET_LIBRARY

include(../../../../openpilotgcslibrary.pri)
#DESTDIR = ../build
SOURCES += mapgraphicitem.cpp \
    opmapwidget.cpp

LIBS += -L../build -lcore -linternals -lcore
HEADERS += mapgraphicitem.h \
    opmapwidget.h 

QT += opengl
QT += network
QT += sql