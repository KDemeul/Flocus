#-------------------------------------------------
#
# Project created by QtCreator 2014-10-28T10:07:01
#
#-------------------------------------------------

QT += core gui

QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Flocus_window
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    centralarea.cpp \
    datavisualizer.cpp \
    fldatahandler.cpp \
    Algorithm/algorithmransac.cpp \
    common.cpp \
    qalgorithm.cpp

HEADERS  += mainwindow.h \
    centralarea.h \
    datavisualizer.h \
    fldatahandler.h \
    Algorithm/algorithmransac.h \
    common.h \
    qalgorithm.h

#INCLUDEPATH += /usr/include/vtk-5.8

#LIBS+= -L/usr/include/vtk-5.8/ -lvtkCommon -lvtksys -lQVTK -lvtkQtChart -lvtkViews -lvtkWidgets -lvtkInfovis -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtkFiltering -lvtkDICOMParser  -lvtkalglib -lvtkverdict -lvtkmetaio -lvtkexoIIc -lvtkftgl -lvtkHybrid

INCLUDEPATH += /usr/include/opencv2

LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc

OTHER_FILES += \
    stylesheet.qss

