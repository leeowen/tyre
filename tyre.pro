#-------------------------------------------------
#
# Project created by QtCreator 2018-11-05T15:40:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tyre
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    renderarea.cpp

HEADERS += \
        mainwindow.h \
    renderarea.h

FORMS += \
        mainwindow.ui


INCLUDEPATH += "/usr/local/Cellar/eigen/3.3.4/include/eigen3" \
                "/usr/local/Cellar/cgal/4.13/include" \
                "/usr/local/Cellar/boost/1.68.0/include" \
                "/usr/local/Cellar/gmp/6.1.2_2/include" \
                "/usr/local/Cellar/mpfr/4.0.1/include"

LIBS += -L/usr/local/Cellar/gmp/6.1.2_2/lib\
        -lgmp \
        -L/usr/local/Cellar/mpfr/4.0.1/lib\
        -lmpfr \
        -L/usr/local/Cellar/cgal/4.13/lib\
        -lCGAL

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
