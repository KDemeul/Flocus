#ifndef FLDATAHANDLER_H
#define FLDATAHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <QMessageBox>


class FlDataHandler
{
public:
    FlDataHandler(QString filename, QWidget *parent);
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

    int*** allPictures;

    bool fileLoaded = false;


private:

};

#endif // FLDATAHANDLER_H
