#include "Visualizers/qalgorithm.h"
#include "Main/centralarea.h"

QAlgorithm::QAlgorithm(QWidget *parent) :
    QWidget(parent)
{
    mMainLayout = new QVBoxLayout;
    mMainLabel= new QLabel("<b>Algorithm</b>");
    mMainLabel->setAlignment(Qt::AlignCenter);

    // Group : RANSAC
    QFormLayout *ransacLayout = new QFormLayout;

    // Slider ROI
    for(int i=0;i<4;i++)
    {
        mSlidersRANSACRec[i] = new QSlider(Qt::Horizontal);
        mSlidersRANSACRec[i]->setRange(0,1000);
    }

    // TEMP
    mSlidersRANSACRec[0]->setValue(0);
    mSlidersRANSACRec[1]->setValue(120);
    mSlidersRANSACRec[2]->setValue(640);
    mSlidersRANSACRec[3]->setValue(80);

    mLabelRANSACRec[0] = new QLabel("PosX");
    mLabelRANSACRec[1] = new QLabel("PosY");
    mLabelRANSACRec[2] = new QLabel("Width");
    mLabelRANSACRec[3] = new QLabel("Height");

    QFormLayout *ROILayout = new QFormLayout;
    for(int i=0;i<4;i++)
    {
        ROILayout->addRow(mLabelRANSACRec[i],mSlidersRANSACRec[i]);
    }
    for(int i=0;i<4;i++)
    {
        connect(mSlidersRANSACRec[i],SIGNAL(actionTriggered(int)),this,SLOT(setROI()));
    }

    QGroupBox *groupROI = new QGroupBox("ROI");
    groupROI->setLayout(ROILayout);


    ransacLayout->addWidget(groupROI);

    // Show ROI
    mButtonShowROi = new QPushButton("Show ROI");
    ransacLayout->addWidget(mButtonShowROi);
    connect(mButtonShowROi,SIGNAL(clicked()),this,SLOT(setROI()));

    // Frame rate
    mSpinRR = new QSpinBox;
    mSpinRR->setRange(1,20);
    QFormLayout *RANSACRateLayout = new QFormLayout;
    RANSACRateLayout->addRow("RANSAC Rate:", mSpinRR);
    QWidget *RANSACRateWidget = new QWidget;
    RANSACRateWidget->setLayout(RANSACRateLayout);
    ransacLayout->addWidget(RANSACRateWidget);

    groupRANSAC = new QGroupBox("RANSAC");
    groupRANSAC->setCheckable(true);
    groupRANSAC->setLayout(ransacLayout);

    connect(groupRANSAC,SIGNAL(toggled(bool)),this,SLOT(toggledRANSAC(bool)));


    // End group : RANSAC

    mMainLayout->addWidget(mMainLabel);
    mMainLayout->addWidget(groupRANSAC);

    this->setLayout(mMainLayout);
}

void QAlgorithm::toggledRANSAC(bool a_isVisible)
{
    groupRANSAC->setChecked(a_isVisible);
    groupRANSAC->setHidden(!a_isVisible);
}

void QAlgorithm::setROI()
{
    CentralArea *parent = (CentralArea*)this->parentWidget();
    parent->setROI(mSlidersRANSACRec[0]->value(),
                    mSlidersRANSACRec[1]->value(),
                    mSlidersRANSACRec[2]->value(),
                    mSlidersRANSACRec[3]->value());
}

void QAlgorithm::setBounds(int a_w, int a_h)
{
    for(int i=0;i<2;i++)
    {
        mSlidersRANSACRec[2*i]->setRange(0,a_w);
        mSlidersRANSACRec[2*i+1]->setRange(0,a_h);
    }
}
