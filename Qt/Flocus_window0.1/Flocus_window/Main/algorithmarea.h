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
#include <QComboBox>
#include <QAction>

class AlgorithmArea : public QWidget
{
    Q_OBJECT
public:
    AlgorithmArea(QWidget *parent = 0);
    void setBounds(int a_w, int a_h);
    QGroupBox* getGroupBoxRansac();
    QGroupBox* getGroupBoxTip();

signals:

public slots:
    void communicateToVisualizer();

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
    QCheckBox   *mButtonDrawRANSAC;

    // Tip detection
    QGroupBox *groupTip;
    QComboBox *mComboTipDirection;

};

#endif // ALGORITHMAREA_H
