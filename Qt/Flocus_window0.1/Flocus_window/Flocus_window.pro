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

SOURCES += \
    Algorithm/algorithmransac.cpp \
    common/common.cpp \
    DataHandling/fldatahandler.cpp \
    Main/algorithmarea.cpp \
    Main/mainwindow.cpp \
    Main/centralarea.cpp \
    Visualizers/datavisualizer.cpp \
    Visualizers/ransacvisualizer.cpp \
    Main/main.cpp

HEADERS  += \
    Algorithm/algorithmransac.h \
    common/common.h \
    DataHandling/fldatahandler.h \
    Main/algorithmarea.h \
    Main/mainwindow.h \
    Main/centralarea.h \
    Visualizers/datavisualizer.h \
    Visualizers/ransacvisualizer.h \

#INCLUDEPATH += /usr/include/vtk-5.8

#LIBS+= -L/usr/include/vtk-5.8/ -lvtkCommon -lvtksys -lQVTK -lvtkQtChart -lvtkViews -lvtkWidgets -lvtkInfovis -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtkFiltering -lvtkDICOMParser  -lvtkalglib -lvtkverdict -lvtkmetaio -lvtkexoIIc -lvtkftgl -lvtkHybrid

INCLUDEPATH += /usr/include/opencv2

LIBS+= -lopencv_core -lopencv_highgui -lopencv_imgproc

OTHER_FILES += \
    stylesheet.qss \
    common/stylesheet.qss

