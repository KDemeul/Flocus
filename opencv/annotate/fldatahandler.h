#ifndef FLDATAHANDLER_H
#define FLDATAHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include <time.h>


class FlDataHandler
{
public:
    FlDataHandler();
    FlDataHandler(std::string filename);
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

    std::string mFilename;

    float*** allPictures;

    bool fileLoaded;


private:

};

#endif // FLDATAHANDLER_H
