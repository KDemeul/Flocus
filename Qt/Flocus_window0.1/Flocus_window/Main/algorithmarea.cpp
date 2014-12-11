#include "Main/algorithmarea.h"
#include "Main/centralarea.h"

AlgorithmArea::AlgorithmArea(QWidget *parent) :
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

    // STARTING VALUE
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
        connect(mSlidersRANSACRec[i],SIGNAL(actionTriggered(int)),this,SLOT(communicateToVisualizer()));
    }

    QGroupBox *groupROI = new QGroupBox("ROI");
    groupROI->setLayout(ROILayout);


    ransacLayout->addWidget(groupROI);

    // Show ROI
    mButtonShowROi = new QPushButton("Show ROI");
    ransacLayout->addWidget(mButtonShowROi);
    connect(mButtonShowROi,SIGNAL(clicked()),this,SLOT(communicateToVisualizer()));

    // RANSAC rate
    mSpinRR = new QSpinBox;
    mSpinRR->setRange(1,20);
    mSpinRR->setValue(10);
    QFormLayout *RANSACRateLayout = new QFormLayout;
    RANSACRateLayout->addRow("RANSAC Rate:", mSpinRR);
    QWidget *RANSACRateWidget = new QWidget;
    RANSACRateWidget->setLayout(RANSACRateLayout);
    ransacLayout->addWidget(RANSACRateWidget);
    connect(mSpinRR,SIGNAL(valueChanged(int)),this,SLOT(communicateToVisualizer()));

    groupRANSAC = new QGroupBox("RANSAC");
    groupRANSAC->setCheckable(true);
    groupRANSAC->setLayout(ransacLayout);

    connect(groupRANSAC,SIGNAL(toggled(bool)),this,SLOT(communicateToVisualizer()));
    // End group : RANSAC

    // Group : TIP
    QFormLayout *tipLayout = new QFormLayout;

    mComboTipDirection = new QComboBox;
    mComboTipDirection->addItem("Right");
    mComboTipDirection->addItem("Up");
    mComboTipDirection->addItem("Left");
    mComboTipDirection->addItem("Down");
    connect(mComboTipDirection,SIGNAL(activated(int)),this,SLOT(communicateToVisualizer()));
    tipLayout->addWidget(mComboTipDirection);

    groupTip = new QGroupBox("TIP");
    groupTip->setCheckable(true);
    groupTip->setLayout(tipLayout);

    connect(groupTip,SIGNAL(toggled(bool)),this,SLOT(communicateToVisualizer()));
    // End group : TIP

    // Group : KALMAN

    QFormLayout *kalmanLayout = new QFormLayout;

    mButtonDisplayMatrix = new QPushButton("Show Matrix");
    connect(mButtonDisplayMatrix,SIGNAL(clicked()),this,SLOT(displayKalmanMatrix()));
    kalmanLayout->addWidget(mButtonDisplayMatrix);

    groupKalman = new QGroupBox("KALMAN");
    groupKalman->setCheckable(true);
    groupKalman->setLayout(kalmanLayout);
    connect(groupKalman,SIGNAL(toggled(bool)),this,SLOT(communicateToVisualizer()));


//    mMatrixFrame = new MatrixFrame(this);
//    CentralArea* parentCentralArea = (CentralArea*)parent;
//    connect(parentCentralArea->getDataVisualizer(),SIGNAL(matrixKalmanUpdated(std::vector<cv::Mat>)),mMatrixFrame,SLOT(setMatrix(std::vector<cv::Mat>)));

    // End group : KALMAN

    mMainLayout->addWidget(mMainLabel);
    mMainLayout->addWidget(groupRANSAC);
    mMainLayout->addWidget(groupTip);
    mMainLayout->addWidget(groupKalman);

    this->setLayout(mMainLayout);

    // INITIAL VALUES
    groupRANSAC->setChecked(false);
    groupTip->setChecked(true);
    groupKalman->setChecked(true);
}

void AlgorithmArea::setBounds(int a_w, int a_h)
{
    for(int i=0;i<2;i++)
    {
        mSlidersRANSACRec[2*i]->setRange(0,a_w);
        mSlidersRANSACRec[2*i+1]->setRange(0,a_h);
    }
}

QGroupBox* AlgorithmArea::getGroupBoxRansac()
{
    return groupRANSAC;
}

QGroupBox* AlgorithmArea::getGroupBoxTip()
{
    return groupTip;
}

QGroupBox* AlgorithmArea::getGroupBoxKalman()
{
    return groupKalman;
}

void AlgorithmArea::displayKalmanMatrix()
{
     CentralArea *parent = (CentralArea*)this->parentWidget();

//     std::vector<cv::Mat> matrix = parent->getKalmanMatrix();
//     mMatrixFrame->setMatrix(matrix);
//     mMatrixFrame->show();
}

void AlgorithmArea::communicateToVisualizer()
{
    CentralArea *parent = (CentralArea*)this->parentWidget();

    // RANSAC
    parent->setRansacParameters(mSlidersRANSACRec[0]->value(),
            mSlidersRANSACRec[1]->value(),
            mSlidersRANSACRec[2]->value(),
            mSlidersRANSACRec[3]->value(),
            groupRANSAC->isChecked(),
            mSpinRR->value());
    groupRANSAC->setHidden(!groupRANSAC->isChecked());

    // TIP
    if(!groupRANSAC->isChecked()){
        groupTip->setChecked(false);
    }
    parent->setTipParameters((ORIENTATION_NEEDLE)mComboTipDirection->currentIndex(),
                             groupTip->isChecked());
    groupTip->setHidden(!groupTip->isChecked());

    // KALMAN
    if(!groupTip->isChecked()) {
        groupKalman->setChecked(false);
    }
    parent->setKalmanParameters(groupKalman->isChecked());
    groupKalman->setHidden(!groupKalman->isChecked());

}
