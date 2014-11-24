#include "fldatahandler.h"
using namespace std;

FlDataHandler::FlDataHandler()
{
    fileLoaded = false;
}

FlDataHandler::FlDataHandler(std::string filename)
{
    mFilename = filename;

    ifstream rawData(filename.c_str(),ios::binary);

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

        allPictures = (float***)malloc(sizeof(float**)*nframes);
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            allPictures[frameCount] = (float**) malloc(h*sizeof(float*));
            for (int x = 0; x < h; ++x) {
                allPictures[frameCount][x] = (float*) malloc(w*sizeof(float));
            }
        }


        float max = -1.0;
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            for (int x = 0; x < h; ++x) {
                for (int y = 0; y < w; ++y) {
                    int tmp = 0;
                    rawData.read((char*)&tmp,sizeof(tmp));
                    if(tmp > max)
                        max = (float)tmp;
                    allPictures[frameCount][x][y] = (float)tmp;
                }
            }
        }

        // NORMALIZE DATA
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            for (int x = 0; x < h; ++x) {
                for (int y = 0; y < w; ++y) {
                    int tmp = allPictures[frameCount][x][y];
                    allPictures[frameCount][x][y] = (tmp / max);
                }
            }
        }
    }
}

