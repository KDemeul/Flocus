#include "roivisualizer.h"

ROIVisualizer::ROIVisualizer(QWidget *parent) :
    QWidget(parent),
    mIsEnabled(true),
    mIndexLastFrameProcessed(-1)
{
    mAlgorithmROI = new AlgorithmROI();
}

void ROIVisualizer::applyAndDraw(cv::Mat *a_imgToDrawOn, cv::Mat *a_imgToProcess, FlGPSHandler *a_flGPSHandler1, FlGPSHandler *a_flGPSHandler2, int a_indexFrame){
    if(!mIsEnabled){
        return;
    }

    DEBUG_MSG("CALL TO APPLYALGORITHM ROI");
    mAlgorithmROI->applyAlgorithm(a_flGPSHandler1,a_flGPSHandler2,a_indexFrame);
    mIndexLastFrameProcessed = a_indexFrame;

    if(mAlgorithmROI->isROIComputed()){
        drawROI(a_imgToDrawOn, mAlgorithmROI->getROI());
    }
}

void ROIVisualizer::drawROI(cv::Mat *a_imgToDrawOn, cv::RotatedRect *a_ROI){
    DEBUG_MSG("IT'S DRAWING !");
}

