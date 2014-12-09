#ifndef ALGORITHMKALMAN_H
#define ALGORITHMKALMAN_H

#include <time.h>
#include "common/common.h"
#include "common/xmlhandler.h"

class AlgorithmKalman
{
public:
    // Standard constructor
    AlgorithmKalman();

    // Method for initialization of Kalman filter
    void init(cv::Mat *A, cv::Mat *B, cv::Mat *Q, cv::Mat *C, cv::Mat *R, cv::Mat *mu0,cv::Mat *Sigma0);

    // Method for applying kalman filter on pic regarding input u and measure z
    void applyAlgorithm(cv::Mat *u, cv::Mat *z, int a_indexFrame);

    // Return if a tip has been found
    bool isTipComputed();

    // Return pointer to tip point
    cv::Point* getTip();

    // Set needle insertion direction
    void setDirection(ORIENTATION_NEEDLE a_ori);

private:
    // tip parameters
    bool mTipComputed;
    cv::Point mPointTip;
    ORIENTATION_NEEDLE mOrientationNeedle;

    // Algorithm attributes & methods
    // x(k+1) = A x(k) + B u(k) + epsilon(k), with cov(epsilon) = Q
    // y(k)   = C x(k) + delta(k), with cov(delta) = R
    cv::Mat A; // Model matrix
    cv::Mat B; // Input matrix
    cv::Mat Q; // Covariance noise in model update
    cv::Mat C; // Measurement matrix
    cv::Mat R; // Covariance noise in measurement

    cv::Mat mu; // Mean
    cv::Mat Sigma; // Variance
};

#endif // ALGORITHMKALMAN_H
