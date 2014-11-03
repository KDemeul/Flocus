#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T10:07:01
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flocus_window
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    centralarea.cpp \
    datavisualizer.cpp \
    fldatahandler.cpp

HEADERS  += mainwindow.h \
    centralarea.h \
    datavisualizer.h \
    fldatahandler.h

LIBS+= -L/usr/include/vtk-5.8/ -lvtkCommon -lvtksys -lQVTK -lvtkQtChart -lvtkViews -lvtkWidgets -lvtkInfovis -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtkFiltering -lvtkDICOMParser  -lvtkalglib -lvtkverdict -lvtkmetaio -lvtkexoIIc -lvtkftgl -lvtkHybrid

INCLUDEPATH += /usr/include/vtk-5.8
