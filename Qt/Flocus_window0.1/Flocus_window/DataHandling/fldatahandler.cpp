#include "DataHandling/fldatahandler.h"
using namespace std;

FlDataHandler::FlDataHandler()
{
    fileLoaded = false;
}

FlDataHandler::FlDataHandler(QString filename,QWidget* parent)
{
    ifstream rawData(filename.toStdString().c_str(),ios::binary);

    if(rawData)
    {
        QProgressDialog progress("Parsing file...", "Cancel", 0, 100,parent);
        int progressValue = 0;
        progress.setWindowModality(Qt::WindowModal);
        progress.show();
        bool canceled = false;

        fileLoaded = true;

        // Header
        rawData.read((char*)&filetype,sizeof(filetype));
        cout << "From image file: filetype: " << filetype << endl;
        rawData.read((char*)&nframes,sizeof(filetype));
        cout << "From image file: nframes: " << nframes << endl;
        rawData.read((char*)&w,sizeof(filetype));
        cout << "From image file: w: " << w << endl;
        rawData.read((char*)&h,sizeof(filetype));
        cout << "From image file: h: " << h << endl;
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
            if(frameCount * 100 / nframes > progressValue)
            {
                progressValue = frameCount*100 / nframes;
                progress.setValue(progressValue);
            }
            if(progress.wasCanceled())
            {
                canceled = true;
                break;
            }
        }

        if(canceled)
        {
             fileLoaded = false;
             return;
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

//        //DEBUG -> print first pic into matlab script
//        ofstream outfile("tmp.m",ios::binary);
//        outfile << "pouet=[\n";

//        for (int x = 0; x < h; ++x) {
//            outfile << "[";
//            for (int y = 0; y < w; ++y) {
//                outfile << allPictures[0][x][y];
//                if(y<w-1)
//                    outfile << ",";
//            }
//            if (x < h - 1)
//                outfile << "];\n";
//        }
//        outfile << "]\n];";
//        outfile.close();


    }
}

