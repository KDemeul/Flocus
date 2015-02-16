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

        // Header
        rawData.read((char*)&filetype,sizeof(filetype));
        DEBUG_MSG("filetype: " << filetype);
        rawData.read((char*)&nframes,sizeof(filetype));
        DEBUG_MSG("nframes: " << nframes);
        rawData.read((char*)&w,sizeof(filetype));
        DEBUG_MSG("w: " << w);
        rawData.read((char*)&h,sizeof(filetype));
        DEBUG_MSG("h: " << h);
        rawData.read((char*)&ss,sizeof(filetype));
        DEBUG_MSG("ss: " << ss);
        rawData.read((char*)&ul[0],sizeof(filetype));
        DEBUG_MSG("ul: " << ul[0]);
        rawData.read((char*)&ul[1],sizeof(filetype));
        DEBUG_MSG("ul: " << ul[1]);
        rawData.read((char*)&ur[0],sizeof(filetype));
        DEBUG_MSG("ur: " << ur[0]);
        rawData.read((char*)&ur[1],sizeof(filetype));
        DEBUG_MSG("ur: " << ur[1]);
        rawData.read((char*)&br[0],sizeof(filetype));
        DEBUG_MSG("br: " << br[0]);
        rawData.read((char*)&br[1],sizeof(filetype));
        DEBUG_MSG("br: " << br[1]);
        rawData.read((char*)&bl[0],sizeof(filetype));
        DEBUG_MSG("bl: " << bl[0]);
        rawData.read((char*)&bl[1],sizeof(filetype));
        DEBUG_MSG("bl: " << bl[1]);
        rawData.read((char*)&probe,sizeof(filetype));
        DEBUG_MSG("probe: " << probe);
        rawData.read((char*)&txf,sizeof(filetype));
        DEBUG_MSG("txf: " << txf);
        rawData.read((char*)&sf,sizeof(filetype));
        DEBUG_MSG("sf: " << sf);
        rawData.read((char*)&dr,sizeof(filetype));
        DEBUG_MSG("dr: " << dr);
        rawData.read((char*)&ld,sizeof(filetype));
        DEBUG_MSG("ld: " << ld);
        rawData.read((char*)&extra,sizeof(filetype));
        DEBUG_MSG("extra: " << extra);

        int frameCount = 0;

        DEBUG_MSG(sizeof(unsigned short int));

        while(rawData.good()){
            double tmp;
            rawData.read((char*)&tmp,sizeof(tmp));
            DEBUG_MSG(frameCount << ": x=" << tmp);
            rawData.read((char*)&tmp,sizeof(tmp));
            DEBUG_MSG(frameCount << ": y=" << tmp);
            rawData.read((char*)&tmp,sizeof(tmp));
            DEBUG_MSG(frameCount << ": z=" << tmp);
            for(int i=0; i < 3;i++){
                for(int j=0; j < 3 ; j++){
                    rawData.read((char*)&tmp,sizeof(tmp));
                    DEBUG_MSG(frameCount << ": s[" << i << "," << j << "] =" << tmp);
                }
            }
            rawData.read((char*)&tmp,sizeof(tmp));
            DEBUG_MSG(frameCount << ": time=" << tmp);
            unsigned short int tmp2;
            rawData.read((char*)&tmp2,sizeof(tmp2));
            DEBUG_MSG(frameCount << ": quality=" << tmp2);

            for(int i= 0; i < 3 ; i++){
                rawData.read((char*)&tmp2,sizeof(tmp2));
                DEBUG_MSG(frameCount << ": zeros=" << tmp2);
            }
            frameCount += 1;
        }
    }
}
//    if(rawData)
//    {
//        QProgressDialog progress("Parsing file...", "Cancel", 0, 100,parent);
//        int progressValue = 0;
//        progress.setWindowModality(Qt::WindowModal);
//        progress.show();
//        bool canceled = false;

//        fileLoaded = true;

//        // Header
//        rawData.read((char*)&filetype,sizeof(filetype));
//        rawData.read((char*)&nframes,sizeof(filetype));
//        rawData.read((char*)&w,sizeof(filetype));
//        rawData.read((char*)&h,sizeof(filetype));
//        rawData.read((char*)&ss,sizeof(filetype));
//        rawData.read((char*)&ul[0],sizeof(filetype));
//        rawData.read((char*)&ul[1],sizeof(filetype));
//        rawData.read((char*)&ur[0],sizeof(filetype));
//        rawData.read((char*)&ur[1],sizeof(filetype));
//        rawData.read((char*)&br[0],sizeof(filetype));
//        rawData.read((char*)&br[1],sizeof(filetype));
//        rawData.read((char*)&bl[0],sizeof(filetype));
//        rawData.read((char*)&bl[1],sizeof(filetype));
//        rawData.read((char*)&probe,sizeof(filetype));
//        rawData.read((char*)&txf,sizeof(filetype));
//        rawData.read((char*)&sf,sizeof(filetype));
//        rawData.read((char*)&dr,sizeof(filetype));
//        rawData.read((char*)&ld,sizeof(filetype));
//        rawData.read((char*)&extra,sizeof(filetype));

//       DEBUG_MSG("filetype:" << filetype);

//       DEBUG_MSG("nframes:" << nframes);

//       DEBUG_MSG("w:" << w);
//       DEBUG_MSG("h:" << h);
//       DEBUG_MSG("ss:" << ss);
//       DEBUG_MSG("probe:" << probe);
//       DEBUG_MSG("dr:" << dr);
//        // Load the datas

//        gps_posx = (float*)malloc(sizeof(float)*nframes);
//        gps_posy = (float*)malloc(sizeof(float)*nframes);;
//        gps_posz  = (float*)malloc(sizeof(float)*nframes);;
//        gps_s  = (float***)malloc(sizeof(float**)*nframes);;
//        gps_time  = (float*)malloc(sizeof(float)*nframes);;
//        gps_quality   = (float*)malloc(sizeof(float)*nframes);;

//        for (int frameCount = 0; frameCount < nframes; ++frameCount) {
//            char tmp;
//            // READ POSX
//            rawData.read((char*)&tmp,8);
//            gps_posx[frameCount] = (float)tmp;
//            DEBUG_MSG(frameCount << ": x= " << (float)tmp);
//            // READ POSY
//            rawData.read((char*)&tmp,8);
//            gps_posx[frameCount] = (float)tmp;
//            // READ POSZ
//            rawData.read((char*)&tmp,8);
//            gps_posx[frameCount] = (float)tmp;
//            // READ S
//            gps_s[frameCount] = (float**)malloc(sizeof(float*)*3);
//            for (int i = 0; i < 3; ++i) {
//                gps_s[frameCount][i] =(float*)malloc(sizeof(float)*3);
//                for(int j=0; j < 3; ++j) {
//                    rawData.read((char*)&tmp,8);
//                    gps_s[frameCount][i][j] = (float)tmp;
//                }
//            }
//            // READ TIME
//            rawData.read((char*)&tmp,8);
//            gps_time[frameCount] = (float)tmp;
//            // READ QUALITY
//            rawData.read((char*)&tmp,2);
//            gps_quality[frameCount] = (float)tmp;
//            // READ 6 ZEROS
//            rawData.read((char*)&tmp,6);

//            if(frameCount * 100 / nframes > progressValue)
//            {
//                progressValue = frameCount*100 / nframes;
//                progress.setValue(progressValue);
//            }
//            if(progress.wasCanceled())
//            {
//                canceled = true;
//                break;
//            }
//        }

//        if(canceled)
//        {
//            fileLoaded = false;
//            return;
//        }

//    }
