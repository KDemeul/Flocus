#ifndef QALGORITHM_H
#define QALGORITHM_H

#include <QWidget>
#include <QSlider>
#include <QLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QSpinBox>

class QAlgorithm : public QWidget
{
    Q_OBJECT
public:
    explicit QAlgorithm(QWidget *parent = 0);
    void setBounds(int a_w, int a_h);
    
signals:
    
public slots:
    void toggledRANSAC(bool a_isVisible);
    void showROI();

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

#endif // QALGORITHM_H
