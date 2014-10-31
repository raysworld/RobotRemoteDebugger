#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T15:32:46
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RobotRemoteDebugger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    RobotClient/RobotObject.cpp \
    RobotClient/Helper.cpp \
    RobotClient/RobotClient.cpp \
    stdafx.cpp

HEADERS  += mainwindow.h \
    RobotClient/RobotObject.h \
    RobotClient/Helper.h \
    RobotClient/RobotClient.h \
    stdafx.h

FORMS    += mainwindow.ui

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = stdafx.h

CONFIG+=debug_and_release
