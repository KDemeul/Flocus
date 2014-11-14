#include "ransacvisualizer.h"

RansacVisualizer::RansacVisualizer(QWidget *parent) :
    QWidget(parent),
    mRansacRate(10),
    mIsEnabled(false)
{
    mAlgorithmRansac = new AlgorithmRansac(2);
}

void RansacVisualizer::applyAndDraw(cv::Mat *a_imgCV, cv::Rect *aROI)
{
    if(!mIsEnabled)
        return;
}

void RansacVisualizer::setRansacRate(int a_ransacRate)
{
    mRansacRate = a_ransacRate;
}

void RansacVisualizer::enable(bool a_isEnable)
{
    mIsEnabled = a_isEnable;
    DEBUG_MSG("TOGGLE RANSAC TO" << a_isEnable);
}

/*
    // TO REMOVE
    if(!mFlDataHandler->fileLoaded)
        return;

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
    */
