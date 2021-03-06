#-------------------------------------------------
#
# Project created by QtCreator 2015-05-16T14:56:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku-Solver
TEMPLATE = app
INCLUDEPATH += C:/Programs/opencv24/opencv_bin2/install/include


SOURCES += main.cpp\
        mainwindow.cpp \
    box1x1widget.cpp \
    box3x3widget.cpp \
    model.cpp \
    modelbox1x1.cpp \
    sudokuinimagedetector.cpp

HEADERS  += mainwindow.h \
    box1x1widget.h \
    box3x3widget.h \
    model.h \
    modelbox1x1.h \
    sudokuinimagedetector.h

FORMS    += mainwindow.ui \
    box.ui
