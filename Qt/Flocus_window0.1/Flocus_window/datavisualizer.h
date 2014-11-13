#ifndef DATAVISUALIZER_H
#define DATAVISUALIZER_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QSlider>
#include <QThread>
#include <QGLWidget>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/cv.h"

// TO REMOVE
#include "Algorithm/algorithmransac.h"

#include "common.h"
#include "fldatahandler.h"
#include <time.h>
class DataVisualizer : public QGLWidget
{
    Q_OBJECT
public:
    explicit DataVisualizer(QWidget *parent = 0);

    void addDrawing();
    void updateImage();
    void setDataHandler(FlDataHandler* a_flDataHandler);
    void drawROI(int a_posX, int a_poxY, int a_width, int a_height);


public slots:
    void fastPreviousFrame();
    void fastNextFrame();
    void nextFrame();
    void previousFrame();
    void firstFrame();
    void lastFrame();
    void play();
    void pause();

protected:
    void initializeGL(); // OpenGL initialization
    void paintGL(); // OpenGL Rendering

    void updateScene();
    void renderImage();

private:
    int mWidth;
    int mHeight;
    bool mSceneChanged;          // Indicates when OpenGL view is to be redrawn
    float mImgRatio;             // height/width ratio
    int mOutH;
    int mOutW;

    int mPosX;                  // Top left X position to render image in the center of widget
    int mPosY;                  // Top left Y position to render image in the center of widget

    cv::Mat mImgCV; // original OpenCV image to be shown
    QImage mImgQt; // Qt image to be rendered

    FlDataHandler* mFlDataHandler;
    AlgorithmRansac *mAlgorithmRansac;

    void setFrame(int a_indexFrame);
    bool mIsPlaying;
    int mIndexCurrentFrame;

    // Image handling function
    void convertToRGB();
    void convertToGrey();

    // Drawing function
    void drawLine(cv::Point start, cv::Point end);
    void drawPoint(cv::Point a_Point);
};

#endif // DATAVISUALIZER_H
