#include "tipvisualizer.h"

// ------------ Constructor ----------------
TipVisualizer::TipVisualizer(QWidget *parent)
    : QWidget(parent),
      mIsEnabled(true),
      mIndexLastFrameProcessed(-1)
{
    mAlgorithmTip = new AlgorithmTip();
}

// ------------ applyAndDraw ----------------
void TipVisualizer::applyAndDraw(cv::Mat *a_imgToDrawOn, cv::Mat *a_imgToProcess, cv::Rect *a_ROI, cv::Mat *a_Hj)
{
    if(!mIsEnabled)
        return;

    mAlgorithmTip->applyAlgorithm(a_imgToProcess,a_ROI,a_Hj);

    if(mAlgorithmTip->isTipComputed())
    {
        drawTip(a_imgToDrawOn,a_ROI, mAlgorithmTip->getTip());
    }
}

// ------------ drawTip ----------------
void TipVisualizer::drawTip(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI, cv::Point *a_ptTip)
{
    cv::circle(*a_imgToDrawOn,cv::Point(a_ROI->x + a_ptTip->x,a_ROI->y + a_ptTip->y),3,COLOR_TIP,-1,0);
}

// ------------ enable ----------------
void TipVisualizer::enable(bool a_isEnable)
{
    mIsEnabled = a_isEnable;
}

// ------------ setDirection ----------------
void TipVisualizer::setDirection(ORIENTATION_NEEDLE a_dir)
{
    mAlgorithmTip->setDirection(a_dir);
}
