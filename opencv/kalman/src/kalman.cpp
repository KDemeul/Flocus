#include "kalman.h"


// ----- Constructor -----
Kalman::Kalman(cv::Mat A, cv::Mat B, cv::Mat Q, cv::Mat C, cv::Mat R) :
A(A),
B(B),
Q(Q),
C(C),
R(R)
{
    // DEBUG_KALMAN("Created kalman filter with:");
    // DEBUG_KALMAN("A=" << A);
    // DEBUG_KALMAN("B=" << B);
    // DEBUG_KALMAN("Q=" << Q);
    // DEBUG_KALMAN("C=" << C);
    // DEBUG_KALMAN("R=" << R);
}

void Kalman::init(cv::Mat mu0,cv::Mat Sigma0)
{
    mu = mu0;
    Sigma = Sigma0;
}

void Kalman::update(cv::Mat z,cv::Mat u)
{
    // DEBUG_KALMAN("Before: mu=" << mu);
    // DEBUG_KALMAN("Before: Sigma=" << Sigma);
    cv::Mat mu_hat = A * mu + B * u;
    // DEBUG_KALMAN("mu_hat=" << mu_hat);
    cv::Mat Sigma_hat = A * Sigma * A.t() + Q;
    cv::Mat K = Sigma_hat * C.t() * (( C * Sigma_hat * C.t() + R).inv());
    mu = mu_hat + K * ( z - C * mu_hat);
    Sigma = Sigma_hat - K * C * Sigma_hat;
    // DEBUG_KALMAN("After: mu=" << mu);
    // DEBUG_KALMAN("After: Sigma=" << Sigma);
}


void Kalman::printInfo()
{
    DEBUG_KALMAN("Mu = ");
    DEBUG_KALMAN(mu);
    DEBUG_KALMAN("Sigma = ");
    DEBUG_KALMAN(Sigma);
}