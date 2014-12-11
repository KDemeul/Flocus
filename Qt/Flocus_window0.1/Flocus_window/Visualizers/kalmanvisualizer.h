#ifndef KALMANVISUALIZER_H
#define KALMANVISUALIZER_H

#include <QWidget>

#include "common/common.h"
#include "Algorithm/algorithmkalman.h"

class KalmanVisualizer : public QWidget
{
    Q_OBJECT
public:
    KalmanVisualizer(QWidget *parent = 0);
    void applyAndDraw(cv::Mat *a_imgToDrawOn, cv::Rect *a_ROI,  cv::Mat *a_u, cv::Mat *a_z, int a_indexFrame);
    void enable(bool a_isEnable);
    void setDirection(ORIENTATION_NEEDLE a_dir);
    std::vector<cv::Mat> getMatrix();

private:
    bool mIsEnabled;
    int mIndexLastFrameProcessed;
    AlgorithmKalman* mAlgorithmKalman;

    //  this draw the tip on imgToDrawOn at ptTip
    void drawTip(cv::Mat* a_imgToDrawOn, cv::Rect *a_ROI, cv::Point *a_ptTip);

};

#endif // KALMANVISUALIZER_H
