#include "algorithmkalman.h"

// ------------ Constructor ----------------
AlgorithmKalman::AlgorithmKalman() :
    mTipComputed(false),
    mPointTip(0,0)
{
}


// ------------ init ----------------
void AlgorithmKalman::init(cv::Mat *a_A, cv::Mat *a_B, cv::Mat *a_Q, cv::Mat *a_C, cv::Mat *a_R, cv::Mat *mu0,cv::Mat *Sigma0)
{
    A = *a_A;
    B = *a_B;
    Q = *a_Q;
    C = *a_C;
    R = *a_R;

    mu = *mu0;
    Sigma = *Sigma0;
}

// ------------ applyAlgorithm ----------------
void AlgorithmKalman::applyAlgorithm(cv::Mat *u, cv::Mat *z, int a_indexFrame)
{
    clock_t time = clock();

    // DEBUG_KALMAN("Before: mu=" << mu);
    // DEBUG_KALMAN("Before: Sigma=" << Sigma);
    cv::Mat mu_hat = A * mu + B * (*u);
    // DEBUG_KALMAN("mu_hat=" << mu_hat);
    cv::Mat Sigma_hat = A * Sigma * A.t() + Q;
    cv::Mat K = Sigma_hat * C.t() * (( C * Sigma_hat * C.t() + R).inv());
    mu = mu_hat + K * ( (*z) - C * mu_hat);
    Sigma = Sigma_hat - K * C * Sigma_hat;
    // DEBUG_KALMAN("After: mu=" << mu);
    // DEBUG_KALMAN("After: Sigma=" << Sigma);

    mTipComputed = true;

    time = clock() - time;
    DEBUG_MSG("KALMAN FILTER RAN IN " << ((float)time*1000)/CLOCKS_PER_SEC << "ms.");

//    XMLhandler::addKalmanInfo(mHjChanged,mPointTip,((float)time*1000)/CLOCKS_PER_SEC,a_indexFrame);
}

// ------------ isTipComputed ----------------
bool AlgorithmKalman::isTipComputed()
{
    return mTipComputed;
}

// ------------ getTip ----------------
cv::Point* AlgorithmKalman::getTip()
{
    return &mPointTip;
}

void AlgorithmKalman::setDirection(ORIENTATION_NEEDLE a_ori)
{
    mOrientationNeedle = a_ori;
}
