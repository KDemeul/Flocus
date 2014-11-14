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
    flDataHandler = new FlDataHandler(((MainWindow*)this->parentWidget())->getFilename(),this);
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

void CentralArea::saveImage(std::string a_filename)
{
    DEBUG_MSG("Saving image under the name " << a_filename);
    cv::imwrite(a_filename, *dataVisualizer->getImgCV());
}

void CentralArea::saveMovie(std::string a_filename)
{
    if(!fileLoaded)
    {
        QMessageBox::critical(this,"No video to record","No video to record.\nLoad a file first.");
        return;
    }

    std::vector<int> movieInfo = dataVisualizer->getMovieInfo();
    if(movieInfo.size() != 4)
    {
        QMessageBox::critical(this,"Error","Not enough parameters for the video retrieved.\n See centralarea.cpp, saveImage() function.");
        return;
    }

    cv::VideoWriter outputVideo(a_filename,CV_FOURCC('M','J','P','G'),movieInfo.at(1),cv::Size(movieInfo.at(2),movieInfo.at(3)));

    QProgressDialog progress("Recording video...", "Abort recording", 0, 100,this);
    progress.setWindowModality(Qt::WindowModal);
    progress.show();
    bool canceled = false;

    dataVisualizer->firstFrame();
    for(int indexFrame=0; indexFrame < movieInfo.at(0) ; indexFrame++)
    {
        progress.setValue(indexFrame*100 / movieInfo.at(0));
        if(progress.wasCanceled())
        {
            canceled = true;
            break;
        }
        outputVideo << *dataVisualizer->getImgCV();
        dataVisualizer->nextFrame();
    }
    progress.setValue(100);
    if(canceled)
    {
        QMessageBox::critical(this,"Error","Writing canceled");
    }
}
