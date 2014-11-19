#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <set>
#include <QGroupBox>
#include <QString>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#else
#define DEBUG_MSG(str) ;
#endif


struct comparePoints {
    bool operator()(const cv::Point & a, const cv::Point & b) {
        return ( a.x<b.x || (a.x == b.x && b.x < b.y) );
    }
};

typedef std::set<cv::Point,comparePoints> SetPoint;

const static cv::Scalar COLOR_TIP = cv::Scalar(255,255,0);
const static cv::Scalar COLOR_INLIERS = cv::Scalar(255,0,0);
const static cv::Scalar COLOR_AXIS = cv::Scalar(0,0,255);
const static float MAX_TIME_RANSAC = 1000.0; // in ms.

#endif // COMMON_H
