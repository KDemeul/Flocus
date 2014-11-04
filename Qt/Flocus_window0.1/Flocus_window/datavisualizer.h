#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QSlider>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"
#include <iostream>


#include "fldatahandler.h"

class DataVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit DataVisualizer(QWidget *parent = 0);
    cv::Mat img;

    QImage* imgQt;
    void updateImage();

private:
    int width;
    int height;


    QLabel* labelImage;

};

#endif // DATAVISUALIZER_H
