#include "datavisualizer.h"

DataVisualizer::DataVisualizer(QWidget *parent)
    : QGLWidget(parent)
    , mWidth(640)
    , mHeight(480)
    , mIndexCurrentFrame(0)
    , mPosX(0)
    , mPosY(0)
    , mOutH(0)
    , mOutW(0)
    , mSceneChanged(false)
{
    this->setMinimumSize(mWidth,mHeight);
    mImgRatio = mWidth/mHeight;
    mFlDataHandler = new FlDataHandler();
}

void DataVisualizer::initializeGL()
{
    makeCurrent();
}

void DataVisualizer::updateScene()
{
    if( mSceneChanged && this->isVisible() )
        updateGL();
}

void DataVisualizer::paintGL()
{
    makeCurrent();

    if( !mSceneChanged )
        return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();

    mSceneChanged = false;
}

void DataVisualizer::renderImage()
{
    makeCurrent();
    glClear(GL_COLOR_BUFFER_BIT);
    if (!mImgQt.isNull())
    {
        glLoadIdentity();
        glPushMatrix();

        int imW = mImgQt.width();
        int imH = mImgQt.height();

        glDrawPixels( imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, mImgQt.bits());
        glPopMatrix();
        glFlush();
    }
}

void DataVisualizer::updateImage(){
    // Create cv image
    cv::Mat image = cv::Mat::zeros(mHeight,mWidth, CV_32FC1);

    for (int x = 0; x < mHeight; ++x) {
        for (int y = 0; y < mWidth; ++y) {
            image.at<float>(x,y) = mFlDataHandler->allPictures[mIndexCurrentFrame][x][mWidth-y];
        }
    }
    image.convertTo(mImgCV, CV_8UC1, 255, 0);

    mImgRatio = (float)image.cols/(float)image.rows;

    // Convert cv image to qimage
    if( mImgCV.channels() == 3)
        mImgQt = QImage((const unsigned char*)(mImgCV.data),
                        mImgCV.cols, mImgCV.rows,
                        mImgCV.step, QImage::Format_RGB888).rgbSwapped();
    else if( mImgCV.channels() == 1)
        mImgQt = QImage((const unsigned char*)(mImgCV.data),
                        mImgCV.cols, mImgCV.rows,
                        mImgCV.step, QImage::Format_Indexed8);

    mImgQt = QGLWidget::convertToGLFormat(mImgQt);

    mSceneChanged = true;
    updateScene();
}

void DataVisualizer::addDrawing(){
    // Convert cv image to qimage
    if( mImgCV.channels() == 3)
        mImgQt = QImage((const unsigned char*)(mImgCV.data),
                        mImgCV.cols, mImgCV.rows,
                        mImgCV.step, QImage::Format_RGB888).rgbSwapped();
    else if( mImgCV.channels() == 1)
        mImgQt = QImage((const unsigned char*)(mImgCV.data),
                        mImgCV.cols, mImgCV.rows,
                        mImgCV.step, QImage::Format_Indexed8);

    mImgQt = QGLWidget::convertToGLFormat(mImgQt);

    mSceneChanged = true;
    updateScene();
}

void DataVisualizer::setDataHandler(FlDataHandler* a_FlDataHandler)
{
    mFlDataHandler = a_FlDataHandler;
}

void DataVisualizer::setFrame(int a_indexFrame)
{
    if(mFlDataHandler->fileLoaded)
    {
        if(a_indexFrame < mFlDataHandler->nframes && a_indexFrame > -1)
            mIndexCurrentFrame = a_indexFrame;
        else
            mIndexCurrentFrame = a_indexFrame < 0 ? 0 : mFlDataHandler->nframes-1;
        updateImage();
    }
}

void DataVisualizer::nextFrame()
{
    int desiredIndex = mIndexCurrentFrame + 1;
    setFrame(desiredIndex);
}

void DataVisualizer::previousFrame()
{
    int desiredIndex = mIndexCurrentFrame - 1;
    setFrame(desiredIndex);
}


void DataVisualizer::firstFrame()
{
    setFrame(0);
}

void DataVisualizer::lastFrame()
{
    setFrame(mFlDataHandler->nframes-1);
}

void DataVisualizer::fastNextFrame()
{
    setFrame(mIndexCurrentFrame+10);
}

void DataVisualizer::fastPreviousFrame()
{
    setFrame(mIndexCurrentFrame-10);
}

void DataVisualizer::play()
{
    if(mFlDataHandler->fileLoaded)
    {
        mIsPlaying = !mIsPlaying;

        while(mIsPlaying && mIndexCurrentFrame < mFlDataHandler->nframes-1)
        {
            clock_t previous_t = clock();
            nextFrame();
            clock_t elapsed = clock() - previous_t;
            clock_t tWait = 1000/mFlDataHandler->ss - 1000 * ((float)elapsed/CLOCKS_PER_SEC);
            cv::waitKey((tWait > 1) ? tWait : 1);
        }
    }
}

void DataVisualizer::pause()
{
    mIsPlaying = false;
}

void DataVisualizer::drawLine(cv::Point start, cv::Point end)
{
    if (!mFlDataHandler->fileLoaded)
        return;

    line(mImgCV,start,end,cv::Scalar(255, 0, 0 ),2,8);

    addDrawing();
}

