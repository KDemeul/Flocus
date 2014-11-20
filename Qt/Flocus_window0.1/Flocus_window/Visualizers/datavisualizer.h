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
#include "Algorithm/algorithmtip.h"

#include <time.h>
#include "common/common.h"
#include "DataHandling/fldatahandler.h"
#include "Visualizers/ransacvisualizer.h"
#include "Visualizers/tipvisualizer.h"

class DataVisualizer : public QGLWidget
{
    Q_OBJECT
public:
    explicit DataVisualizer(QWidget *parent = 0);

    void addDrawing();
    void updateImage();
    void setDataHandler(FlDataHandler* a_flDataHandler);
    cv::Rect* getROI();
    void setRansacParameters(int a_posX, int a_poxY, int a_width, int a_height, bool a_isEnable, int a_ransacRate);
    void setTipParameters(ORIENTATION_NEEDLE a_dir, bool a_isEnable);
    cv::Mat* getImgCV();
    std::vector<int> getMovieInfo();

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
    // Display attributes
    int mWidth, mHeight, mPosX, mPosY; // Width, Height, top left X, top left Y position to render image in the center of widget
    bool mSceneChanged;          // Indicates when OpenGL view is to be redrawn
    float mImgRatio;             // height/width ratio
    cv::Mat mImgCV; // original OpenCV image to be shown
    QImage mImgQt; // Qt image to be rendered

    // Data -- algorithm attributes
    FlDataHandler* mFlDataHandler;
    RansacVisualizer *mRansacVisualizer;
    cv::Rect *mROI;
    cv::Mat mImgForProcessing;

    TipVisualizer *mTipVisualizer;

    // Player
    void onFrame();
    void setFrame(int a_indexFrame);
    bool mIsPlaying;
    int mIndexCurrentFrame;

    // Image handling function
    void convertToRGB();
    void convertToGrey();

    // Drawing function
    void drawLine(cv::Point start, cv::Point end);
    void drawPoint(cv::Point a_Point);
    void drawROI();
};

#endif // DATAVISUALIZER_H
