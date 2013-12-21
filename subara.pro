#-------------------------------------------------
#
# Project created by QtCreator 2013-12-20T13:46:23
#
#-------------------------------------------------

QT += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = subara
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oauth/authorize.cpp \
    config/initialize.cpp \
    config/config.cpp

HEADERS  += mainwindow.hpp \
    config/config.hpp \
    oauth/authorize.hpp \
    config/initialize.hpp

CONFIG += c++11

OTHER_FILES += \
    viewer/viewer.html.txt \
    viewer/viewer.css.txt
