#include "kalmanvisualizer.h"

// ------------ Constructor ----------------
KalmanVisualizer::KalmanVisualizer(QWidget *parent)
    : QWidget(parent),
      mIsEnabled(true),
      mIndexLastFrameProcessed(-1)
{
    mAlgorithmKalman = new AlgorithmKalman();
}

// ------------ applyAndDraw ----------------
void KalmanVisualizer::applyAndDraw(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI,  cv::Mat *a_u, cv::Mat *a_z, int a_indexFrame)
{
    if(!mIsEnabled)
        return;

    mAlgorithmKalman->applyAlgorithm(a_u,a_z,a_indexFrame);

    if(mAlgorithmKalman->isTipComputed())
    {
        drawTip(a_imgToDrawOn,a_ROI, mAlgorithmKalman->getTip());
    }
}

// ------------ drawTip ----------------
void KalmanVisualizer::drawTip(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI, cv::Point *a_ptTip)
{
    cv::circle(*a_imgToDrawOn,cv::Point(a_ROI->x + a_ptTip->x,a_ROI->y + a_ptTip->y),3,COLOR_KALMAN,-1,0);
}

// ------------ enable ----------------
void KalmanVisualizer::enable(bool a_isEnable)
{
    mIsEnabled = a_isEnable;
}

// ------------ setDirection ----------------
void KalmanVisualizer::setDirection(ORIENTATION_NEEDLE a_dir)
{
    mAlgorithmKalman->setDirection(a_dir);
}
