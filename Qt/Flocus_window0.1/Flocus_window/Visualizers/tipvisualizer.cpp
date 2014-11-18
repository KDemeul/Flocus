#include "tipvisualizer.h"

// ------------ Constructor ----------------
TipVisualizer::TipVisualizer(QWidget *parent) :
    QWidget(parent),
    mIsEnabled(true),
    mIndexLastFrameProcessed(-1)
{
    mAlgorithmTip = new AlgorithmTip();
}

// ------------ applyAndDraw ----------------
void TipVisualizer::applyAndDraw(cv::Mat *a_imgCV, cv::Mat *a_Hj)
{
    if(!mIsEnabled)
        return;

    mAlgorithmTip->applyAlgorithm(a_imgCV, a_Hj);

    if(mAlgorithmTip->isTipComputed())
    {
        drawTip(a_imgCV,mAlgorithmTip->getTip());
    }
}

// ------------ drawTip ----------------
void TipVisualizer::drawTip(cv::Mat *a_imgCV, cv::Point *a_ptTip)
{
    cv::circle(*a_imgCV,*a_ptTip,3,COLOR_TIP,-1,0);
}
