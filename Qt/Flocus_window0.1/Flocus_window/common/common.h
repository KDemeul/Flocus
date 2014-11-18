#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/features2d/features2d.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <QGroupBox>

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


#endif // COMMON_H
