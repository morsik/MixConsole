#-------------------------------------------------
#
# Project created by QtCreator 2013-12-28T00:46:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MixConsole
TEMPLATE = app

LIBS += -ljack

SOURCES += src/main.cpp\
        src/MainWindow.cpp \
    src/MeterWidget.cpp \
    src/JackAudio.cpp \
    src/utils.cpp

HEADERS  += src/MainWindow.h \
    src/MeterWidget.h \
    src/JackAudio.h \
    src/utils.h

FORMS    += ui/MainWindow.ui
