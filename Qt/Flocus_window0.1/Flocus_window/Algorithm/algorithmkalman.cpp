#include "algorithmkalman.h"

// ------------ Constructor ----------------
AlgorithmKalman::AlgorithmKalman() :
    mTipComputed(false),
    mPointTip(0,0)
{
}


// ------------ init ----------------
void AlgorithmKalman::init(cv::Mat *a_A, cv::Mat *a_B, cv::Mat *a_Q, cv::Mat *a_C, cv::Mat *a_R, cv::Mat *a_mu0,cv::Mat *a_Sigma0)
{
    A0 = *a_A;
    B0 = *a_B;
    Q0 = *a_Q;
    C0 = *a_C;
    R0 = *a_R;
    mu0 = *a_mu0;
    Sigma0 = *a_Sigma0;
    this->reinit();
}

// ------------ reinit ----------------
void AlgorithmKalman::reinit()
{
    A0.copyTo(A);
    B0.copyTo(B);
    Q0.copyTo(Q);
    C0.copyTo(C);
    R0.copyTo(R);
    mu0.copyTo(mu);
    Sigma0.copyTo(Sigma);

    mKalmanInit = 0;
}

// ------------ applyAlgorithm ----------------
void AlgorithmKalman::applyAlgorithm(cv::Mat *u, cv::Mat *z, int a_indexFrame)
{
    clock_t time = clock();

    if(mKalmanInit==0){
        mu.at<double>(2,0) -= z->at<double>(0,0);
        mu.at<double>(3,0) -= z->at<double>(1,0);
    } else if (mKalmanInit==1) {
        mu.at<double>(0,0) += z->at<double>(0,0);
        mu.at<double>(1,0) += z->at<double>(1,0);
        mu.at<double>(2,0) += z->at<double>(0,0);
        mu.at<double>(3,0) += z->at<double>(1,0);
    } else {
        cv::Mat mu_hat = A * mu + B * (*u);
        cv::Mat Sigma_hat = A * Sigma * A.t() + Q;
        cv::Mat K = Sigma_hat * C.t() * (( C * Sigma_hat * C.t() + R).inv());
        mu = mu_hat + K * ( (*z) - C * mu_hat);
        Sigma = Sigma_hat - K * C * Sigma_hat;
    }

    mKalmanInit += mKalmanInit > 1 ? 0 : 1;
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
    mPointTip = cv::Point(mu.at<double>(0,0),mu.at<double>(1,0));
    return &mPointTip;
}

void AlgorithmKalman::setDirection(ORIENTATION_NEEDLE a_ori)
{
    mOrientationNeedle = a_ori;
}
