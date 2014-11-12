#ifndef COMMON_H
#define COMMON_H

#include <opencv2/core/core.hpp>
#include <set>

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << std::setprecision(18) << str << "\n";
#else
#define DEBUG_MSG(str) ;
#endif


struct comparePoints {
    bool operator()(const cv::Point & a, const cv::Point & b) {
        return ( a.x<b.x && a.y<b.y );
    }
};

typedef std::set<cv::Point,comparePoints> SetPoint;

#endif // COMMON_H
