#include "flgpshandler.h"

FlGPSHandler::FlGPSHandler()
{
    fileLoaded = false;
}

FlGPSHandler::FlGPSHandler(QString filename,QWidget* parent)
{
    ifstream rawData(filename.toStdString().c_str(),ios::binary);

    if(rawData){
        fileLoaded = true;
        fileIntegrity = true;
        // Header
        rawData.read((char*)&filetype,sizeof(filetype));
        // DEBUG_MSG("filetype: " << filetype);
        rawData.read((char*)&nframes,sizeof(filetype));
        // DEBUG_MSG("nframes: " << nframes);
        rawData.read((char*)&w,sizeof(filetype));
        // DEBUG_MSG("w: " << w);
        rawData.read((char*)&h,sizeof(filetype));
        // DEBUG_MSG("h: " << h);
        rawData.read((char*)&ss,sizeof(filetype));
        // DEBUG_MSG("ss: " << ss);
        rawData.read((char*)&ul[0],sizeof(filetype));
        // DEBUG_MSG("ul: " << ul[0]);
        rawData.read((char*)&ul[1],sizeof(filetype));
        // DEBUG_MSG("ul: " << ul[1]);
        rawData.read((char*)&ur[0],sizeof(filetype));
        // DEBUG_MSG("ur: " << ur[0]);
        rawData.read((char*)&ur[1],sizeof(filetype));
        // DEBUG_MSG("ur: " << ur[1]);
        rawData.read((char*)&br[0],sizeof(filetype));
        // DEBUG_MSG("br: " << br[0]);
        rawData.read((char*)&br[1],sizeof(filetype));
        // DEBUG_MSG("br: " << br[1]);
        rawData.read((char*)&bl[0],sizeof(filetype));
        // DEBUG_MSG("bl: " << bl[0]);
        rawData.read((char*)&bl[1],sizeof(filetype));
        // DEBUG_MSG("bl: " << bl[1]);
        rawData.read((char*)&probe,sizeof(filetype));
        // DEBUG_MSG("probe: " << probe);
        rawData.read((char*)&txf,sizeof(filetype));
        // DEBUG_MSG("txf: " << txf);
        rawData.read((char*)&sf,sizeof(filetype));
        // DEBUG_MSG("sf: " << sf);
        rawData.read((char*)&dr,sizeof(filetype));
        // DEBUG_MSG("dr: " << dr);
        rawData.read((char*)&ld,sizeof(filetype));
        // DEBUG_MSG("ld: " << ld);
        rawData.read((char*)&extra,sizeof(filetype));
        // DEBUG_MSG("extra: " << extra);

        // Alloc memory
        gps_posx = (float*) malloc(sizeof(float)*nframes);   // 8 bytes for double
        gps_posy = (float*) malloc(sizeof(float)*nframes);
        gps_posz = (float*) malloc(sizeof(float)*nframes);
        gps_a = (float*) malloc(sizeof(float)*nframes);
        gps_e = (float*) malloc(sizeof(float)*nframes);
        gps_r = (float*) malloc(sizeof(float)*nframes);
        gps_s = (float***) malloc(sizeof(float**)*nframes);    // position matrix
        gps_q = (float***) malloc(sizeof(float**)*nframes);
        gps_time = (float*) malloc(sizeof(float)*nframes);
        gps_quality = (float*) malloc(sizeof(float)*nframes);

        // FILL DATAS
        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
            double tmp;
            rawData.read((char*)&tmp,sizeof(tmp));
            gps_posx[frameCount] = (float) tmp;
            rawData.read((char*)&tmp,sizeof(tmp));;
            gps_posy[frameCount] = (float) tmp;
            rawData.read((char*)&tmp,sizeof(tmp));;
            gps_posz[frameCount] = (float) tmp;
            rawData.read((char*)&tmp,sizeof(tmp));
            gps_a[frameCount] = (float) tmp;
            rawData.read((char*)&tmp,sizeof(tmp));
            gps_e[frameCount] = (float) tmp;
            rawData.read((char*)&tmp,sizeof(tmp));
            gps_r[frameCount] = (float) tmp;

            // ROTATION MATRIX
            gps_s[frameCount] = (float**) malloc(sizeof(float*)*3);
            for(int i=0; i < 3;i++){
                gps_s[frameCount][i] = (float*) malloc(sizeof(float)*3);
                for(int j=0; j < 3 ; j++){
                    rawData.read((char*)&tmp,sizeof(tmp));
                    gps_s[frameCount][i][j] = (float) tmp;
                 }
            }

            // Q MATRIX
            gps_q[frameCount] = (float**) malloc(sizeof(float*)*2);
            for(int i=0; i < 2 ; i++){
                gps_q[frameCount][i] = (float*) malloc(sizeof(float)*2);
                for(int j=0; j < 2 ; j++){
                    rawData.read((char*)&tmp,sizeof(tmp));
                    gps_q[frameCount][i][j] = (float) tmp;
                 }
            }

            rawData.read((char*)&tmp,sizeof(tmp));
            gps_time[frameCount] = (float) tmp;
            unsigned short tmp2;
            rawData.read((char*)&tmp2,2);
            gps_quality[frameCount] = (float) tmp;

            // READ ZEROES
            unsigned short tmp3;
            for(int i= 0; i < 3 ; i++){
                rawData.read((char*)&tmp3,2);
                if((float)tmp3 != 0){
                    fileIntegrity = false;
                    DEBUG_MSG("GPS DATA WRONG AT FRAME " << frameCount);
                }
            }
        }
        if(fileIntegrity){
            DEBUG_MSG("" << filename.toStdString().c_str() << " read sucessfully!");
        }
    }
}
