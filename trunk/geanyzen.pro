#-------------------------------------------------
#
# Project created by QtCreator 2011-03-17T22:50:42
#
#-------------------------------------------------

QT       -= core gui

LIBS += -lpython2.6

TARGET = geanyzen
DESTDIR = build
OBJECTS_DIR = tmp
TEMPLATE = lib

DEFINES += UNTITLED_LIBRARY

SOURCES += src/geanyzen.c
CONFIG += plugin
CONFIG += link_pkgconfig
PKGCONFIG += geany

zen.files    += zencoding
zen.path     += /usr/lib/geany-plugins/geanyzen

plugin.files += build/libgeanyzen.so
plugin.path  += /usr/lib/geany

plugin1.files += build/libgeanyzen.so
plugin1.path  += /usr/lib/geany-plugins/geanyzen

INSTALLS     += plugin
INSTALLS     += zen
INSTALLS     += plugin1
