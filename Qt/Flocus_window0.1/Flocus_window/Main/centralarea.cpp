#include "Main/centralarea.h"
#include "Main/mainwindow.h"

CentralArea::CentralArea(QWidget *parent)
    : QWidget(parent)
    , fileLoaded(false)
{
    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // Create dataVisualizer layout
    QVBoxLayout *dataVizualizerLayout = new QVBoxLayout;

    dataVisualizer = new DataVisualizer(this);
    dataVizualizerLayout->addWidget(dataVisualizer);
    dataVizualizerLayout->addStretch(2000);

    mainLayout->addStretch(2000);
    mainLayout->addLayout(dataVizualizerLayout);

    // Create right layout
    rightLayout   = new QVBoxLayout;

    // Algorithm layout
    mAlgorithmArea = new AlgorithmArea(this);
    rightLayout->addWidget(mAlgorithmArea);


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
    buttonFastForward          = new QPushButton(QIcon("../../icons/glyphicons_176_forward.png"),QString());
    buttonFastBackward         = new QPushButton(QIcon("../../icons/glyphicons_172_rewind.png"),QString());

    buttonArray = (QPushButton**) malloc(sizeof(QPushButton*) * 8);
    buttonArray[0] = buttonFastBackward;
    buttonArray[1] = buttonPlay;
    buttonArray[2] = buttonPause;
    buttonArray[3] = buttonFastForward;
    buttonArray[4] = buttonBackward;
    buttonArray[5] = buttonStepBackward;
    buttonArray[6] = buttonStepForward ;
    buttonArray[7] = buttonForward;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            layoutVisualizationPart->addWidget(buttonArray[i*4+j],i,j);
        }
    }

//    rightLayout->addLayout(layoutAlgorithm);
    rightLayout->addWidget(labelVisualizationPart);
    rightLayout->addLayout(layoutVisualizationPart);
    rightLayout->addStretch(2000);

    mainLayout->addLayout(rightLayout);
    mainLayout->addStretch(2000);

    this->setLayout(mainLayout);

    // CONNECTION
    connect(buttonFastBackward,SIGNAL(clicked()),dataVisualizer,SLOT(fastPreviousFrame()));
    connect(buttonFastForward,SIGNAL(clicked()),dataVisualizer,SLOT(fastNextFrame()));
    connect(buttonBackward,SIGNAL(clicked()),dataVisualizer,SLOT(firstFrame()));
    connect(buttonForward,SIGNAL(clicked()),dataVisualizer,SLOT(lastFrame()));
    connect(buttonStepBackward,SIGNAL(clicked()),dataVisualizer,SLOT(previousFrame()));
    connect(buttonStepForward,SIGNAL(clicked()),dataVisualizer,SLOT(nextFrame()));
    connect(buttonPlay,SIGNAL(clicked()),dataVisualizer,SLOT(play()));

    //Any button (except play) throw the pause even
    for (int i = 0; i < 8; ++i) {
        if(i!=1)
            connect(buttonArray[i],SIGNAL(clicked()),dataVisualizer,SLOT(pause()));
    }
}

void CentralArea::updateFile()
{
    flDataHandler = new FlDataHandler(((MainWindow*)this->parentWidget())->getFilename());
    if(flDataHandler->fileLoaded)
    {
        fileLoaded = true;
        dataVisualizer->setDataHandler(flDataHandler);
        dataVisualizer->updateImage();
        dataVisualizer->addDrawing();
        if(mAlgorithmArea->getGroupBoxRansac()->isChecked())
            mAlgorithmArea->communicateToVisualizer();
        mAlgorithmArea->setBounds(flDataHandler->w,flDataHandler->h);
    }
    else
    {
        QMessageBox::critical(this, "Error while opening", "File can not be load.");
    }
}

void CentralArea::setRansacParameters(int a_posX, int a_poxY, int a_width, int a_height, bool a_isEnable, int a_ransacRate)
{
   dataVisualizer->setRansacParameters(a_posX,a_poxY,a_width,a_height,a_isEnable,a_ransacRate);
}

AlgorithmArea* CentralArea::getAlgorithmArea()
{
    return mAlgorithmArea;
}

