QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LexiEditor
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           lexicalanalyzer.cpp

HEADERS += mainwindow.h \
           lexicalanalyzer.h
