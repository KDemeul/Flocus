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
        drawTip(a_imgToDrawOn,mAlgorithmTip->getTip());
    }
}

// ------------ drawTip ----------------
void TipVisualizer::drawTip(cv::Mat *a_imgToDrawOn, cv::Point *a_ptTip)
{
    cv::circle(*a_imgToDrawOn,*a_ptTip,3,COLOR_TIP,-1,0);
}
