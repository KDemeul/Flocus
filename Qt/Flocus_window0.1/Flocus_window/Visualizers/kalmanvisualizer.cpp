#include "kalmanvisualizer.h"

// ------------ Constructor ----------------
KalmanVisualizer::KalmanVisualizer(QWidget *parent)
    : QWidget(parent),
      mIsEnabled(true),
      mIndexLastFrameProcessed(-1)
{
    // // Create kalman
    mAlgorithmKalman = new AlgorithmKalman();
    double sigmaQ = 1e-5;
    double sigmaR = 1e-1;

    cv::Mat A = (cv::Mat_<double>(4,4) <<
                 1, 0, 1, 0,
                 0, 1, 0, 1,
                 0, 0, 1, 0,
                 0, 0, 0, 1);

    cv::Mat B = (cv::Mat_<double>(4,1) << 0, 0, 0, 0);

    cv::Mat Q= (cv::Mat_<double>(4,4) <<
                sigmaQ, 0, 0, 0,
                0, sigmaQ, 0, 0,
                0, 0, sigmaQ, 0,
                0, 0, 0, sigmaQ);

    common::addNoise(&Q,sigmaQ/1000);

    cv::Mat C = (cv::Mat_<double>(2,4) <<
                 1, 0, 0, 0,
                 0, 1, 0, 0);

    cv::Mat R = (cv::Mat_<double>(2,2) <<
                 sigmaR, 0,
                 0, sigmaR);
    common::addNoise(&R,sigmaR/1000);

    cv::Mat mu0 = (cv::Mat_<double>(4,1) << 0,0,0,0);

    cv::Mat Sigma0 = (cv::Mat_<double>(4,4) <<
                      0, 0, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0);
    common::addNoise(&Sigma0,0.01);

    mAlgorithmKalman->init(&A,&B,&Q,&C,&R,&mu0,&Sigma0);

}

// ------------ applyAndDraw ----------------
void KalmanVisualizer::applyAndDraw(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI,  cv::Mat *a_u, cv::Mat *a_z, int a_indexFrame)
{
    if(!mIsEnabled){
        return;
    }

    if(abs(mIndexLastFrameProcessed-a_indexFrame) > 1){
        mAlgorithmKalman->reinit();
    }

    mAlgorithmKalman->applyAlgorithm(a_u,a_z,a_indexFrame);
    mIndexLastFrameProcessed = a_indexFrame;

    if(mAlgorithmKalman->isTipComputed()){
        drawTip(a_imgToDrawOn,a_ROI, mAlgorithmKalman->getTip());
    }
}

// ------------ drawTip ----------------
void KalmanVisualizer::drawTip(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI, cv::Point *a_ptTip)
{
    cv::Point tipOnPic(a_ROI->x + a_ptTip->x,a_ROI->y + a_ptTip->y);
    cv::line(*a_imgToDrawOn,tipOnPic,tipOnPic+cv::Point(20,20),COLOR_KALMAN,1);
    cv::putText(*a_imgToDrawOn,"K",tipOnPic+cv::Point(20,30),cv::FONT_HERSHEY_SIMPLEX,0.5,COLOR_KALMAN);
    cv::circle(*a_imgToDrawOn,tipOnPic,3,COLOR_KALMAN,-1,0);
}

// ------------ enable ----------------
void KalmanVisualizer::enable(bool a_isEnable)
{
    mIsEnabled = a_isEnable;
}

// ------------ getMatrix ----------------
std::vector<cv::Mat> KalmanVisualizer::getMatrix()
{
    return mAlgorithmKalman->getMatrix();
}
