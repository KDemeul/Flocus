#include "centralarea.h"
#include "mainwindow.h"

CentralArea::CentralArea(QWidget *parent) : QWidget(parent)
{
    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    dataVisualizer = new DataVisualizer(this);
    mainLayout->addWidget(dataVisualizer);

    // Create right layout
    QVBoxLayout *rightLayout   = new QVBoxLayout;

    // Algorithm layout
    labelAlgorithmPart         = new QLabel("<b>Algorithm</b>");
    labelAlgorithmPart->setAlignment(Qt::AlignCenter);
    buttonRANSAC               = new QCheckBox("RANSAC");
    buttonKALMAN               = new QCheckBox("KALMAN");
    buttonKALMAN->setEnabled(false);
    connect(buttonRANSAC,SIGNAL(toggled(bool)),buttonKALMAN,SLOT(setEnabled(bool)));
    connect(buttonRANSAC,SIGNAL(toggled(bool)),buttonKALMAN,SLOT(setChecked(bool)));

    // Visualization layout
    labelVisualizationPart     = new QLabel("<b>Visualization</b>");
    labelVisualizationPart->setAlignment(Qt::AlignCenter);
    layoutVisualizationPart    = new QGridLayout;
    buttonBackward             = new QPushButton(QIcon("../../icons/glyphicons_171_fast_backward.png"),QString());
    buttonStepBackward         = new QPushButton(QIcon("../../icons/glyphicons_170_step_backward.png"),QString());
    buttonPlay                 = new QPushButton(QIcon("../../icons/glyphicons_173_play.png"),QString());
    buttonPause                = new QPushButton(QIcon("../../icons/glyphicons_174_pause.png"),QString());
    buttonStepForward          = new QPushButton(QIcon("../../icons/glyphicons_178_step_forward.png"),QString());
    buttonForward              = new QPushButton(QIcon("../../icons/glyphicons_177_fast_forward.png"),QString());
    layoutVisualizationPart->addWidget(buttonBackward,1,0);
    layoutVisualizationPart->addWidget(buttonStepBackward,1,1);
    layoutVisualizationPart->addWidget(buttonPlay,0,1);
    layoutVisualizationPart->addWidget(buttonPause,0,2);
    layoutVisualizationPart->addWidget(buttonStepForward,1,2);
    layoutVisualizationPart->addWidget(buttonForward,1,3);

    rightLayout->addWidget(labelAlgorithmPart);
    rightLayout->addWidget(buttonRANSAC);
    rightLayout->addWidget(buttonKALMAN);

    rightLayout->addWidget(labelVisualizationPart);
    rightLayout->addLayout(layoutVisualizationPart);

    rightLayout->addStretch(2000);

    mainLayout->addLayout(rightLayout);
    mainLayout->addStretch(2000);

    this->setLayout(mainLayout);

    connect(buttonPlay,SIGNAL(clicked()),this,SLOT(updateImage()));
}

QCheckBox* CentralArea::getButtonRANSAC()
{
    return buttonRANSAC;
}

QCheckBox* CentralArea::getButtonKALMAN()
{
    return buttonKALMAN;
}

void CentralArea::updateImage()
{
    flDataHandler = new FlDataHandler(((MainWindow*)this->parentWidget())->getFilename(),this);
    if(flDataHandler->fileLoaded)
    {
        dataVisualizer->setSize(flDataHandler->w,flDataHandler->h);
        dataVisualizer->cImage = flDataHandler->allPictures[0];
        dataVisualizer->updateImage();
    }
}
