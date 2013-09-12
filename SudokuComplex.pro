#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T12:16:42
#
#-------------------------------------------------

QT       += core gui
QT += widgets

TARGET = SudokuComplex
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    BoardDelegate.cpp \
    BoardModel.cpp \
    Cell.cpp \
    SudokuClass.cpp \
    RandomSudokuGenerator.cpp \
    SudokuStructure.cpp \
    HelperClass.cpp \
    numberpick.cpp \
    resultsdialog.cpp \
    newdialog.cpp \
    about.cpp \
    sudokusettings.cpp

HEADERS  += mainwindow.h \
    BoardDelegate.h \
    BoardModel.h \
    Cell.h \
    SudokuClass.h \
    RandomSudokuGenerator.h \
    SudokuStructure.h \
    Types.h \
    HelperClass.h \
    numberpick.h \
    resultsdialog.h \
    newdialog.h \
    about.h \
    sudokusettings.h

FORMS    += mainwindow.ui \
    numberpick.ui \
    resultsdialog.ui \
    newdialog.ui \
    about.ui
