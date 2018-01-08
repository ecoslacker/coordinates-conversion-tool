#-------------------------------------------------
#
# Project created by QtCreator 2014-05-08T00:10:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Coordinates
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ellipsoid.cpp \
    utmcoordinates.cpp \
    gcscoordinates.cpp \
    aboutdialog.cpp \
    csv/csv.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    ellipsoid.h \
    utmcoordinates.h \
    gcscoordinates.h \
    aboutdialog.h \
    csv/csv.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    helpdialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = Coordinates.rc

win32:RC_ICONS += Coordinates.ico
