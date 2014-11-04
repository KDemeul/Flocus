#include "fldatahandler.h"
using namespace std;

FlDataHandler::FlDataHandler(QString filename,QWidget* parent)
{
    ifstream rawData(filename.toStdString().c_str(),ios::binary);
//    ifstream rawData(filename.toStdString().c_str(),ios::binary);
    if(rawData)
    {
        fileLoaded = true;

        // Header
        rawData.read((char*)&filetype,sizeof(filetype));
        rawData.read((char*)&nframes,sizeof(filetype));
        rawData.read((char*)&w,sizeof(filetype));
        rawData.read((char*)&h,sizeof(filetype));
        rawData.read((char*)&ss,sizeof(filetype));
        rawData.read((char*)&ul[0],sizeof(filetype));
        rawData.read((char*)&ul[1],sizeof(filetype));
        rawData.read((char*)&ur[0],sizeof(filetype));
        rawData.read((char*)&ur[1],sizeof(filetype));
        rawData.read((char*)&br[0],sizeof(filetype));
        rawData.read((char*)&br[1],sizeof(filetype));
        rawData.read((char*)&bl[0],sizeof(filetype));
        rawData.read((char*)&bl[1],sizeof(filetype));
        rawData.read((char*)&probe,sizeof(filetype));
        rawData.read((char*)&txf,sizeof(filetype));
        rawData.read((char*)&sf,sizeof(filetype));
        rawData.read((char*)&dr,sizeof(filetype));
        rawData.read((char*)&ld,sizeof(filetype));
        rawData.read((char*)&extra,sizeof(filetype));

        // Load the datas
        int max = -1;

        allPictures = (int***)malloc(sizeof(int**)*nframes);
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            allPictures[frameCount] = (int**) malloc(h*sizeof(int*));
            for (int x = 0; x < h; ++x) {
                allPictures[frameCount][x] = (int*) malloc(w*sizeof(int));
            }
        }

        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            for (int x = 0; x < h; ++x) {
                for (int y = 0; y < w; ++y) {
                    int tmp = 0;
                    rawData.read((char*)&tmp,sizeof(tmp));
                    if(tmp > max)
                        max = tmp;
                    allPictures[frameCount][x][y] = tmp;
                }
            }
        }

        // NORMALIZE DATA
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            for (int x = 0; x < h; ++x) {
                for (int y = 0; y < w; ++y) {
                    allPictures[frameCount][x][y] *= 255;
                    allPictures[frameCount][x][y] /= max;
                }
            }
        }
    }
}

