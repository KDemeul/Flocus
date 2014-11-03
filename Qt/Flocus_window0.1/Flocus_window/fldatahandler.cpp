#include "fldatahandler.h"
using namespace std;

FlDataHandler::FlDataHandler(QString filename,QWidget* parent)
{
    ifstream rawData(filename.toStdString().c_str(),ios::binary);
    if(rawData)
    {
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
        allPictures = (char**)malloc(sizeof(char*)*nframes);
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            allPictures[frameCount] = (char*) malloc(w*h*sizeof(int));
            rawData.read(allPictures[frameCount],w*h*sizeof(int));
        }
        QMessageBox::information(parent,"Success","File " + filename + " successfully open.");
        fileLoaded = true;
    }
    else
    {
        QMessageBox::critical(parent, "Error while opening", "File " + filename + " can not be load.");
    }
}

