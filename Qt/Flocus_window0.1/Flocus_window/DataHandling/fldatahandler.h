#ifndef FLDATAHANDLER_H
#define FLDATAHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QProgressDialog>

#include <time.h>


class FlDataHandler
{
public:
    FlDataHandler();
    FlDataHandler(QString filename,QWidget* parent = 0);
    int nbField;
    int filetype;
    int nframes;
    int w;
    int h;
    int ss;
    int ul[2];
    int ur[2];
    int br[2];
    int bl[2];
    int probe;
    int txf;
    int sf;
    int dr;
    int ld;
    int extra;

    float*** allPictures;

    bool fileLoaded;


private:

};

#endif // FLDATAHANDLER_H
