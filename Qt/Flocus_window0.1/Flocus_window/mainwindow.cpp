#include "mainwindow.h"
#include "centralarea.h"

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

    QAction *actionClose = new QAction("&Close", this);
    menuFile->addAction(actionClose);
    actionClose->setIcon(QIcon("../../icons/glyphicons_063_power.png"));

    //---> Sub menu algorithm
    QAction *actionRANSAC = new QAction("&RANSAC",this);
    menuAlgorithm->addAction(actionRANSAC);
    actionRANSAC->setCheckable(true);

    QAction *actionKalman = new QAction("&KALMAN",this);
    menuAlgorithm->addAction(actionKalman);
    actionKalman->setCheckable(true);

    //---> Sub menu help
    QAction *actionAbout = new QAction("&About",this);
    menuHelp->addAction(actionAbout);
    actionAbout->setIcon(QIcon("../../icons/glyphicons_082_roundabout.png"));

    //---> Slot connections

    // Quit
    // TODO FIX THIS
//    connect(actionClose, SIGNAL(triggered()),this, SLOT(quit()));

    // Open
    connect(actionOpen,SIGNAL(triggered()),this,SLOT(openFileDialogue()));

    // Save
    // TODO

    // About
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(openAboutDialogue()));

    // RANSAC & KALMAN
    connect(actionRANSAC,SIGNAL(toggled(bool)),actionKalman,SLOT(setEnabled(bool)));
    connect(actionRANSAC,SIGNAL(toggled(bool)),actionKalman,SLOT(setChecked(bool)));
    connect(actionRANSAC,SIGNAL(toggled(bool)),centralArea->getButtonRANSAC(),SLOT(setChecked(bool)));
    connect(actionKalman,SIGNAL(toggled(bool)),centralArea->getButtonKALMAN(),SLOT(setChecked(bool)));
    connect(centralArea->getButtonRANSAC(),SIGNAL(toggled(bool)),actionRANSAC,SLOT(setChecked(bool)));
    connect(centralArea->getButtonKALMAN(),SIGNAL(toggled(bool)),actionKalman,SLOT(setChecked(bool)));
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

void MainWindow::openFileDialogue()
{
    filename = QFileDialog::getOpenFileName(this, "Open file","/home/kiki/Documents/Prog/LIRMM/Flocus/data/10-21-2014-Generic/","US Data (*.b32 *.b8)");
    centralArea->updateImage();
}

QString MainWindow::getFilename(){
    return this->filename;
}
