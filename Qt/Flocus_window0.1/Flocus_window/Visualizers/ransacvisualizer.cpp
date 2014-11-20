#include "ransacvisualizer.h"

RansacVisualizer::RansacVisualizer(QWidget *parent) :
    QWidget(parent),
    mRansacRate(10),
    mIsEnabled(false),
    mIndexLastFrameProcessed(-1)
{
    mAlgorithmRansac = new AlgorithmRansac(2);
}

void RansacVisualizer::applyAndDraw(cv::Mat *a_imgToDrawOn, cv::Mat *a_imgToProcess, cv::Rect *a_ROI, int a_indexFrame)
{
    if(!mIsEnabled)
        return;

    if(a_indexFrame % mRansacRate != 0 && abs(mIndexLastFrameProcessed-a_indexFrame) < 21)
    {
        if(mAlgorithmRansac->isModelComputed())
            drawInliers(a_imgToDrawOn,a_ROI);
        return;
    }

    mAlgorithmRansac->applyAlgorithm(a_imgToProcess, a_ROI);

    if(mAlgorithmRansac->isModelComputed())
    {
        mIndexLastFrameProcessed = a_indexFrame;
        drawInliers(a_imgToDrawOn,a_ROI);
    }

}

void RansacVisualizer::drawInliers(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI)
{
    std::vector<cv::Point> inliers = mAlgorithmRansac->getInliers();
    for(std::vector<cv::Point>::iterator it = inliers.begin(); it != inliers.end() ; it++)
    {
        drawPoint(a_imgToDrawOn,cv::Point(a_ROI->x,a_ROI->y) + cv::Point(it->y,it->x));
    }

    cv::Mat* paramCurve = mAlgorithmRansac->getParamCurve();
    cv::Mat T1 = (cv::Mat_<double>(2,1) << 1, -1000);
    cv::Mat T2 = (cv::Mat_<double>(2,1) << 1, 1000);
    cv::Mat M1mat = *paramCurve * T1;
    cv::Mat M2mat = *paramCurve * T2;
    cv::Point M1(M1mat.at<double>(1,0),M1mat.at<double>(0,0));
    cv::Point M2(M2mat.at<double>(1,0),M2mat.at<double>(0,0));
    drawLine(a_imgToDrawOn,cv::Point(a_ROI->x,a_ROI->y) + M1,cv::Point(a_ROI->x,a_ROI->y) + M2);
}

void RansacVisualizer::setRansacRate(int a_ransacRate)
{
    mRansacRate = a_ransacRate;
}

void RansacVisualizer::enable(bool a_isEnable)
{
    mIsEnabled = a_isEnable;
}

void RansacVisualizer::drawLine(cv::Mat* a_imgToDrawOn, cv::Point start, cv::Point end)
{
    line(*a_imgToDrawOn,start,end,COLOR_AXIS,1,8);
}

void RansacVisualizer::drawPoint(cv::Mat* a_imgToDrawOn, cv::Point a_Point)
{
    cv::circle(*a_imgToDrawOn,a_Point,1,COLOR_INLIERS,-1,0);
}

cv::Mat* RansacVisualizer::getParamCurve()
{
    return mAlgorithmRansac->getParamCurve();
}
