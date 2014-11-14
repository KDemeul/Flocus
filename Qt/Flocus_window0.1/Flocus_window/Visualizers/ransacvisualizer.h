#ifndef RANSACVISUALIZER_H
#define RANSACVISUALIZER_H

#include <QWidget>
#include "Algorithm/algorithmransac.h"

class RansacVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit RansacVisualizer(QWidget *parent = 0);
    void applyAndDraw(cv::Mat *a_imgCV, cv::Rect *aROI);
    
signals:
    
public slots:
    void setRansacRate(int a_ransacRate);
    void enable(bool a_isEnable);

private:
    AlgorithmRansac *mAlgorithmRansac;
    int mRansacRate;
    bool mIsEnabled;
    
};

#endif // RANSACVISUALIZER_H
