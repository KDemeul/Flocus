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
    mAlgorithmRansac = new AlgorithmRansac(2);
}

// PAN INITIALIZATION, DEFINITION AND UPDATE
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

// VIDEO CONTROL
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
    // TO REMOVE
    updateImage();

    mAlgorithmRansac->applyAlgorithm(mImgCV, cv::Rect(0,120,640,80));

    DEBUG_MSG("RANSAC HAS RUN");
    DEBUG_MSG((mAlgorithmRansac->isModelComputed() ? "A model has been computed " : "No model has been found"));

    if(mAlgorithmRansac->isModelComputed())
    {
        convertToRGB();

        std::vector<cv::Point> inliers = mAlgorithmRansac->getInliers();
        for(std::vector<cv::Point>::iterator it = inliers.begin(); it != inliers.end() ; it++)
        {
            drawPoint(cv::Point(0,120) + cv::Point(it->y,it->x));
        }

        cv::Mat paramCurve = mAlgorithmRansac->getParamCurve();
        cv::Mat T1 = (cv::Mat_<double>(2,1) << 1, -1000);
        cv::Mat T2 = (cv::Mat_<double>(2,1) << 1, 1000);
        cv::Mat M1mat = paramCurve * T1;
        cv::Mat M2mat = paramCurve * T2;
        cv::Point M1(M1mat.at<double>(1,0),M1mat.at<double>(0,0));
        cv::Point M2(M2mat.at<double>(1,0),M2mat.at<double>(0,0));
        drawLine(cv::Point(0,120) + M1,cv::Point(0,120) + M2);

        addDrawing();
    }

    // TO REMOVE


    mIsPlaying = false;
}


// DRAWING FUNCTION
void DataVisualizer::drawLine(cv::Point start, cv::Point end)
{
    if (!mFlDataHandler->fileLoaded)
        return;

    line(mImgCV,start,end,cv::Scalar(255, 0, 0 ),1,8);
}

void DataVisualizer::drawPoint(cv::Point a_Point)
{
    if (!mFlDataHandler->fileLoaded)
        return;

    cv::circle(mImgCV,a_Point,2,cv::Scalar(0, 0, 255),-1,0);
}

void DataVisualizer::drawROI(int a_posX, int a_poxY, int a_width, int a_height)
{
    if(!mFlDataHandler->fileLoaded)
        return;

    updateImage();

    convertToRGB();

    int x = a_posX;
    int y = a_poxY;
    int w = a_width;
    int h = a_height;
    w = x+w >= mImgCV.cols ? mImgCV.cols - x : w;
    h = y+h >= mImgCV.rows ? mImgCV.rows - y : h;
    cv::rectangle(mImgCV,cv::Rect(x,y,w,h),cv::Scalar(0,255,0),1);

    addDrawing();
}

void DataVisualizer::convertToRGB()
{
    if(mImgCV.type()!=CV_8UC3)
    {
        cv::Mat img_rgb(mImgCV.size(), CV_8UC3);
        cv::cvtColor(mImgCV, img_rgb, CV_GRAY2RGB);
        mImgCV = img_rgb;
    }
}

void DataVisualizer::convertToGrey()
{
    if(!mImgCV.type()!=CV_8UC1)
    {
        cv::Mat img_grey(mImgCV.size(), CV_8UC1);
        cv::cvtColor(mImgCV, img_grey, CV_RGB2GRAY);
        mImgCV = img_grey;
    }
}
