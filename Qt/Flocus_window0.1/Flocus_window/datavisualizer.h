#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QSlider>

//#include <QVTKWidget.h>
//#include <vtkImageData.h>
//#include <vtkSmartPointer.h>
//#include <vtkImageImport.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include <vtkImageActor.h>


#include "fldatahandler.h"

class DataVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit DataVisualizer(QWidget *parent = 0);
    void setSize(int width,int height);
    char* cImage;

signals:

public slots:
    void updateImage();

private:
//    QVTKWidget *widget;
    int width;
    int height;
//    vtkSmartPointer<vtkImageImport> imageImport;

};

#endif // DATAVISUALIZER_H
