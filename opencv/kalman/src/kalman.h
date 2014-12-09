#ifndef KALMAN_H
#define KALMAN_H

#include <iostream>
#include <vector>
#include <string>
#include <time.h>


#define DEBUG_K
#ifdef DEBUG_K
#define DEBUG_KALMAN(str) std::cout << "KALMAN: " << str << std::endl;
#else
#define DEBUG_KALMAN(str) ;
#endif


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class Kalman
{
public:
  // Standard constructor
  Kalman(cv::Mat A, cv::Mat B, cv::Mat R, cv::Mat C, cv::Mat Q);
  void update(cv::Mat z,cv::Mat u);
  void init(cv::Mat mu0,cv::Mat Sigma0);
  void printInfo();

  cv::Mat mu;
  cv::Mat Sigma;

private:
  // x(k+1) = A x(k) + B u(k) + epsilon(k), with cov(epsilon) = Q
  // y(k)   = C x(k) + delta(k), with cov(delta) = R
  cv::Mat A; // Model matrix
  cv::Mat B; // Input matrix
  cv::Mat Q; // Covariance noise in model update
  cv::Mat C; // Measurement matrix
  cv::Mat R; // Covariance noise in measurement
};

#endif // KALMAN_H
