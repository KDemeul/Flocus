#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "centralarea.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void openAboutDialogue();
    void openFileDialogue();

private:
    CentralArea *centralArea;
    QString filename;
};

#endif // MAINWINDOW_H
