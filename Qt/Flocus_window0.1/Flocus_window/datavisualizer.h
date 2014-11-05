#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QSlider>
#include <QThread>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"
#include <iostream>
#include <qapplication.h>

#include "fldatahandler.h"

#include <time.h>

class DataVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit DataVisualizer(QWidget *parent = 0);
    cv::Mat img;

    QImage imgQt;
    void updateImage();
    void setDataHandler(FlDataHandler* a_flDataHandler);


public slots:
    void nextFrame();
    void previousFrame();
    void firstFrame();
    void lastFrame();
    void play();
    void pause();

private:
    int width;
    int height;

    FlDataHandler* flDataHandler;

    QLabel* labelImage;

    void setFrame(int a_indexFrame);
    bool isPlaying;
    int indexCurrentFrame;

    void sleep(unsigned int mseconds);
};

#endif // DATAVISUALIZER_H
