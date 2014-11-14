#ifndef ALGORITHMAREA_H
#define ALGORITHMAREA_H

#include <QWidget>
#include <QSlider>
#include <QLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>

class AlgorithmArea : public QWidget
{
    Q_OBJECT
public:
    AlgorithmArea(QWidget *parent = 0);
    void setBounds(int a_w, int a_h);
    QGroupBox* getGroupBoxRansac();

signals:

public slots:
    void toggledRANSAC(bool a_isVisible);
    void setROI();

private:
    // GLOBAL ATTRIBUTES
    QVBoxLayout *mMainLayout;
    QLabel      *mMainLabel;

    // RANSAC
    QGroupBox   *groupRANSAC;
    QCheckBox   *mButtonRANSAC;
    QSlider     *mSlidersRANSACRec[4];
    QLabel      *mLabelRANSACRec[4];
    QPushButton *mButtonShowROi;
    QSpinBox    *mSpinRR;
};

#endif // ALGORITHMAREA_H
