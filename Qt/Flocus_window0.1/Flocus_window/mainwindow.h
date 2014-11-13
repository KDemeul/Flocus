#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

#include "fldatahandler.h"
#include "centralarea.h"
#include "qalgorithm.h"

class CentralArea;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    QString getFilename();

private slots:
    void openAboutDialogue();
    void openFileDialogue();

private:
    CentralArea* centralArea;
    QString filename;

};

#endif // MAINWINDOW_H
