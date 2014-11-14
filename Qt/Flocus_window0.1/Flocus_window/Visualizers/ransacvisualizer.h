#ifndef RANSACVISUALIZER_H
#define RANSACVISUALIZER_H

#include <QWidget>
#include "Algorithm/algorithmransac.h"

class RansacVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit RansacVisualizer(QWidget *parent = 0);
    void applyAndDraw(cv::Mat *a_imgCV, cv::Rect *a_ROI, int a_indexFrame);
    
signals:
    
public slots:
    void setRansacRate(int a_ransacRate);
    void enable(bool a_isEnable);

private:
    AlgorithmRansac *mAlgorithmRansac;
    int mRansacRate;
    bool mIsEnabled;

    int mIndexLastFrameProcessed;

    void drawInliers(cv::Mat* a_imgCV, cv::Rect *a_ROI);

    void drawPoint(cv::Mat* a_imgCV, cv::Point a_Point);
    void drawLine(cv::Mat* a_imgCV, cv::Point start, cv::Point end);
    
};

#endif // RANSACVISUALIZER_H
