#ifndef FLGPSHANDLER_H
#define FLGPSHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QProgressDialog>

#include <time.h>
#include "common/common.h"

using namespace std;

class FlGPSHandler
{
public:
    FlGPSHandler();
    FlGPSHandler(QString filename,QWidget* parent = 0);
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

    float *gps_posx;   // 8 bytes for double
    float *gps_posy;
    float *gps_posz;
    float *gps_a;
    float *gps_e;
    float *gps_r;
    float ***gps_s;    // position matrix
    float ***gps_q;
    float *gps_time;
    float *gps_quality; // 2 bytes for unsigned short

    bool fileLoaded;

    bool fileIntegrity;


private:
};

#endif // FLGPSHANDLER_H
