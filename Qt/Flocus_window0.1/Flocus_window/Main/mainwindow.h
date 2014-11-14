#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include "DataHandling/fldatahandler.h"
#include "Main/centralarea.h"
#include "Main/algorithmarea.h"

class CentralArea;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    QString getFilename();

private slots:
    void openAboutDialogue();
    void openFileDialogue();
    void saveImageDialogue();
    void saveMovieDialogue();

private:
    CentralArea* centralArea;
    QString filename;

};

#endif // MAINWINDOW_H
