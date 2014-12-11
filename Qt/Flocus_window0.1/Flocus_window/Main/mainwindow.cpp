#include "Main/mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    /*--------------
     * Central area
     *--------------*/

    centralArea = new CentralArea(this);
    setCentralWidget(centralArea);

    /*--------------
     * Menu
     *--------------*/

    //---> Main menu
    QMenu *menuFile = menuBar()->addMenu("&File");
    QMenu *menuAlgorithm = menuBar()->addMenu("&Algorithm");
    QMenu *menuHelp = menuBar()->addMenu("&Help");

    //---> Sub menu file
    QAction *actionOpen = new QAction("&Open",this);
    menuFile->addAction(actionOpen);
    actionOpen->setIcon(QIcon("../../icons/glyphicons_144_folder_open.png"));

    QMenu *menuSave = new QMenu("&Save",this);
    menuFile->addMenu(menuSave);
    menuSave->setIcon(QIcon("../../icons/glyphicons_443_floppy_disk.png"));
    QAction *actionSaveImage = new QAction("Save &Image",this);
    QAction *actionSaveMovie = new QAction("Save &Movie",this);
    menuSave->addAction(actionSaveImage);
    menuSave->addAction(actionSaveMovie);

    menuFile->addSeparator();

    QAction *actionExtract = new QAction("&Extract",this);
    menuFile->addAction(actionExtract);
    actionExtract->setIcon(QIcon("../../icons/glyphicons_041_charts.png"));
    mExtractDialog = new ExtractDialog(this);

    menuFile->addSeparator();

    QAction *actionClose = new QAction("&Close", this);
    menuFile->addAction(actionClose);
    actionClose->setIcon(QIcon("../../icons/glyphicons_063_power.png"));

    //---> Sub menu algorithm
    QAction *actionRANSAC = new QAction("&RANSAC",this);
    menuAlgorithm->addAction(actionRANSAC);
    actionRANSAC->setCheckable(true);

    QAction *actionTip = new QAction("&TIP",this);
    menuAlgorithm->addAction(actionTip);
    actionTip->setCheckable(true);
    actionTip->setEnabled(false);

    QAction *actionKalman = new QAction("&KALMAN",this);
    menuAlgorithm->addAction(actionKalman);
    actionKalman->setCheckable(true);
    actionKalman->setEnabled(false);

    //---> Sub menu help
    QAction *actionAbout = new QAction("&About",this);
    menuHelp->addAction(actionAbout);
    actionAbout->setIcon(QIcon("../../icons/glyphicons_082_roundabout.png"));

    //---> Slot connections

    // Quit
    connect(actionClose, SIGNAL(triggered()),qApp, SLOT(quit()));

    // Open
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(openFileDialogue()));

    // Save
    connect(actionSaveImage,SIGNAL(triggered()),this,SLOT(saveImageDialogue()));
    connect(actionSaveMovie,SIGNAL(triggered()),this,SLOT(saveMovieDialogue()));

    // Extract
    connect(actionExtract,SIGNAL(triggered()),this,SLOT(openExtractDialogue()));

    // About
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(openAboutDialogue()));

    // RANSAC
    connect(actionRANSAC,SIGNAL(toggled(bool)),centralArea->getAlgorithmArea()->getGroupBoxRansac(),SLOT(setChecked(bool)));
    connect(centralArea->getAlgorithmArea()->getGroupBoxRansac(),SIGNAL(toggled(bool)),actionRANSAC,SLOT(setChecked(bool)));
    connect(actionRANSAC,SIGNAL(toggled(bool)),actionTip,SLOT(setEnabled(bool)));

    // TIP
    connect(actionTip,SIGNAL(toggled(bool)),centralArea->getAlgorithmArea()->getGroupBoxTip(),SLOT(setChecked(bool)));
    connect(centralArea->getAlgorithmArea()->getGroupBoxTip(),SIGNAL(toggled(bool)),actionTip,SLOT(setChecked(bool)));

    // KALMAN
    connect(actionKalman,SIGNAL(toggled(bool)),centralArea->getAlgorithmArea()->getGroupBoxKalman(),SLOT(setChecked(bool)));
    connect(centralArea->getAlgorithmArea()->getGroupBoxKalman(),SIGNAL(toggled(bool)),actionKalman,SLOT(setChecked(bool)));
    connect(actionTip,SIGNAL(toggled(bool)),actionKalman,SLOT(setEnabled(bool)));
}


void MainWindow::openAboutDialogue()
{
    QString message = "<b>Flocus 0.1.0 (opensource)</b><br><br>"
            "Flocus stands for <b>F</b>ull needle <b>LOC</b>alization in <b>U</b>ltra<b>S</b>ound images.<br><br>"
            "Flocus applies various image processing algorithms <br>to data acquired by a SonixTouch ultrasound machine.<br><br>"
            "This project is included in the CAMI labex.";

    QString title = "About Flocus";
    QDialog *msg = new QDialog(this);
    msg->setWindowTitle(title);
    QVBoxLayout *layoutMsg = new QVBoxLayout;

    QLabel *iconLabel = new QLabel;
    QImage icon("../../fig/flocus_icon.png");
    iconLabel->setPixmap(QPixmap::fromImage(icon));
    iconLabel->setAlignment(Qt::AlignCenter);
    layoutMsg->addWidget(iconLabel);

    QLabel *text = new QLabel;
    text->setText(message);
    text->setAlignment(Qt::AlignCenter);
    layoutMsg->addWidget(text);

    QPushButton *buttonOK = new QPushButton("OK");
    buttonOK->setMaximumWidth(50);
    layoutMsg->addWidget(buttonOK,0,Qt::AlignCenter);

    msg->setLayout(layoutMsg);

    connect(buttonOK,SIGNAL(clicked()),msg,SLOT(accept()));

    msg->exec();
}

void MainWindow::openExtractDialogue()
{
    mExtractDialog->exec();
}

void MainWindow::openFileDialogue()
{
    filename = QFileDialog::getOpenFileName(this, "Open file","/home/kilian/Documents/Flocus/data/10-21-2014-Generic/","US Data (*.b32 *.b8)",0,QFileDialog::DontUseNativeDialog);
    centralArea->updateFile();
}

void MainWindow::saveImageDialogue()
{
    std::string picFilename = (QFileDialog::getSaveFileName(this,"Save file","/home/kilian/Documents/Flocus/data/saved/","Image (*.jpg *.jpeg *.png)")).toStdString();
    if(picFilename.size()<4)
    {
        centralArea->saveImage(picFilename + ".jpg");
    }
    else
    {
        std::string suffix = picFilename.substr(picFilename.size()-4,4);
        DEBUG_MSG("suffix: " << suffix);
        if(suffix.compare(".jpg") == 0)
        {
            centralArea->saveImage(picFilename);
        }
        else if (suffix.compare("jpeg") == 0)
        {
            centralArea->saveImage(picFilename.substr(0,picFilename.size()-5) + ".jpg");
        }
        else if (suffix.compare(".png") == 0)
        {
            centralArea->saveImage(picFilename);
        }
        else
        {
            centralArea->saveImage(picFilename + ".jpg");
        }
    }
}

void MainWindow::saveMovieDialogue()
{
    int doRecording = QMessageBox::question(this,"Warning","You're about to record a movie sequence.\n This could take a while.\n Are you sure?", QMessageBox ::Yes | QMessageBox::No);
    if(doRecording == QMessageBox::No)
        return;

    std::string movieFilename = QFileDialog::getSaveFileName(this,"Save file", "/home/kilian/Documents/Flocus/data/saved/", "avi (*.avi)", 0,QFileDialog::DontUseNativeDialog).toStdString();

    if(movieFilename.size()<4)
    {
        centralArea->saveMovie(movieFilename);
    }
    else
    {
        std::string suffix = movieFilename.substr(movieFilename.size()-4,4);
        DEBUG_MSG("suffix: " << suffix);
        if(suffix.compare(".avi") == 0)
        {
            centralArea->saveMovie(movieFilename);
        }
        else
        {
            centralArea->saveMovie(movieFilename + ".avi");
        }
    }
}

QString MainWindow::getFilename(){
    return this->filename;
}
