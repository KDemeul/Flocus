#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <cmath>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "pugixml.hpp"

#include "kalman.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#else
#define DEBUG_MSG(str) ;
#endif

// #define PRINT
#ifdef PRINT
#define PRINT_MSG(str) std::cout << str << std::endl;
#else
#define PRINT_MSG(str) ;
#endif

#define forv(vec,it) for(std::vector<cv::Point>::iterator it = vec.begin(); it != vec.end() ; ++it)
#define DEBUG_VEC(vec) for(std::vector<cv::Point>::iterator it = vec.begin() ; it != vec.end() ; ++it){ DEBUG_MSG(*it) } 

#define WIDTH 600
#define HEIGHT 600
#define YMAX 500
#define YMIN 0

            //FUNCTION
void readMeasureFile(std::string filename);
void readGroundTruth(std::string filename);

            // FILE HANDLING
std::vector<cv::Point> vecMeasures;
std::vector<cv::Point> vecGround;
std::vector<cv::Point> vecEstimated;
pugi::xml_document xmlDoc;

            // VISU
cv::Mat output;
cv::Scalar COLOR_KALMAN = cv::Scalar(255,0,0); // BLUE
cv::Scalar COLOR_MEASURE = cv::Scalar(0,255,0); // GREEN
cv::Scalar COLOR_TRUTH = cv::Scalar(0,0,255);  // RED

            // Kalman
Kalman* kalman;

void displayResults(cv::Mat *output);
void displayError(cv::Mat *output);
void animate();

void addNoise(cv::Mat *a_mat, double a_amp){
    for(int i = 0 ; i < a_mat->rows; i++){
        for (int j = 0; j < a_mat->cols; ++j){
             a_mat->at<double>(i,j) += (((double)(rand() % 100)) / 100.0)*a_amp;
        }
    }
}

int main(int argc, char **argv)
{
    DEBUG_MSG("------");
    double sigmaQ = 1e-5;
    double sigmaR = 1e-1;

    if(argc < 2) {
        std::cout << "Input: measure, ground truth, sigmaQ, sigmaR" << std::endl;
        return 0;
    } else if (argc == 5) {
        std::stringstream ssQ;
        ssQ << argv[3];
        ssQ >> sigmaQ;
        std::stringstream ssR;
        ssR << argv[4];
        ssR >> sigmaR;
    }
    DEBUG_MSG("sigmaQ = " << sigmaQ << ", sigmaR = " << sigmaR);
            // READ MEASUREMENTS
    std::string filename1 = argv[1];
    std::string filename2 = argv[2];
    readMeasureFile(filename1);
    int N = vecMeasures.size();
    readGroundTruth(filename2);

    // // Create kalman
    cv::Mat A = (cv::Mat_<double>(4,4) << 
        1, 0, 1, 0,
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1);

    cv::Mat B = (cv::Mat_<double>(4,1) << 0, 0, 0, 0);

    cv::Mat Q= (cv::Mat_<double>(4,4) <<
      sigmaQ, 0, 0, 0,
      0, sigmaQ, 0, 0,
      0, 0, sigmaQ, 0,
      0, 0, 0, sigmaQ);

    addNoise(&Q,sigmaQ/1000);

    cv::Mat C = (cv::Mat_<double>(2,4) << 
        1, 0, 0, 0, 
        0, 1, 0, 0);

    cv::Mat R = (cv::Mat_<double>(2,2) << 
        sigmaR, 0,
        0, sigmaR);
    addNoise(&R,sigmaR/1000);

    kalman = new Kalman(A,B,Q,C,R);

    cv::Mat mu0 = (cv::Mat_<double>(4,1) << 0,0,0,0);
    mu0.at<double>(0,0) = vecMeasures.begin()->x;
    mu0.at<double>(1,0) = vecMeasures.begin()->y;
    mu0.at<double>(2,0) = vecMeasures.at(1).x - vecMeasures.at(0).x;
    mu0.at<double>(3,0) = vecMeasures.at(1).y - vecMeasures.at(0).y;

    cv::Mat Sigma0 = (cv::Mat_<double>(4,4) <<
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0,
      0, 0, 0, 0);
    addNoise(&Sigma0,0.01);

    kalman->init(mu0,Sigma0);

    size_t index = 0;
    for (std::vector<cv::Point>::iterator it = vecMeasures.begin(); it != vecMeasures.end(); ++it)
    {
        cv::Mat z_k = (cv::Mat_<double>(2,1) << it->x, it->y);
        cv::Mat u_k = (cv::Mat_<double>(1,1) << 2);
        kalman->update(z_k,u_k);
        cv::Mat mu = kalman->mu;
        cv::Mat Sigma = kalman->Sigma;
        vecEstimated.push_back(cv::Point(mu.at<double>(0,0),mu.at<double>(1,0)));
        // kalman->printInfo() ;
        index++; 
    }

    cv::Mat output = cv::Mat::zeros(WIDTH,HEIGHT,CV_8UC3);
    cv::circle(output,cv::Point(0,0),10000,cv::Scalar(255,255,255),-1,0);
    
    // animate();
    displayResults(&output);
    displayError(&output);  

    imshow("ouput",output);
    cv::waitKey(0);
}

void animate(){
    size_t N = vecMeasures.size();
    cv::Mat output = cv::Mat::zeros(WIDTH,HEIGHT,CV_8UC3);
    cv::circle(output,cv::Point(0,0),10000,cv::Scalar(255,255,255),-1,0);

    for(size_t i = 0; i < N; i++){
        cv::circle(output,vecMeasures.at(i),3,COLOR_MEASURE);
        cv::circle(output,vecGround.at(i),3,COLOR_TRUTH);
        cv::circle(output,vecEstimated.at(i),3,COLOR_KALMAN);
        imshow("output",output);
        cv::waitKey(50);
        cv::circle(output,cv::Point(0,0),10000,cv::Scalar(255,255,255),-1,0);
    }
}

void displayResults(cv::Mat *output){
    cv::Point prev_posx_ground;
    cv::Point prev_posx_kalman;
    cv::Point prev_posx_measure;
    cv::Point prev_posy_ground;
    cv::Point prev_posy_kalman;
    cv::Point prev_posy_measure;

    size_t N = vecMeasures.size();

    for(size_t i=0 ; i < N ; i++){
        // Measurement
        cv::Point posx_measure;
        cv::Point posy_measure;
        posx_measure.x = i* WIDTH / N;
        posy_measure.x = i* WIDTH / N;
        posx_measure.y = HEIGHT-(vecMeasures.at(i).x-YMIN)*HEIGHT/(YMAX-YMIN);
        posy_measure.y = HEIGHT-(vecMeasures.at(i).y - YMIN)*HEIGHT/(YMAX-YMIN);
        if(i>0){
            cv::line(*output,prev_posx_measure,posx_measure,COLOR_MEASURE);
            cv::line(*output,prev_posy_measure,posy_measure,COLOR_MEASURE);
        }

        // KALMAN
        cv::Point posx_kalman;
        cv::Point posy_kalman;
        posx_kalman.x = i* WIDTH / N;
        posy_kalman.x = i* WIDTH / N;
        posx_kalman.y = HEIGHT-(vecEstimated.at(i).x-YMIN)*HEIGHT/(YMAX-YMIN);
        posy_kalman.y = HEIGHT-(vecEstimated.at(i).y - YMIN)*HEIGHT/(YMAX-YMIN);
        if(i>0){
            cv::line(*output,prev_posx_kalman,posx_kalman,COLOR_KALMAN);
            cv::line(*output,prev_posy_kalman,posy_kalman,COLOR_KALMAN);
        }

        // GROUND
        cv::Point posx_ground;
        cv::Point posy_ground;
        posx_ground.x = i* WIDTH / N;
        posy_ground.x = i* WIDTH / N;
        posx_ground.y = HEIGHT-(vecGround.at(i).x-YMIN)*HEIGHT/(YMAX-YMIN);
        posy_ground.y = HEIGHT-(vecGround.at(i).y - YMIN)*HEIGHT/(YMAX-YMIN);
        if(i>0){
            cv::line(*output,prev_posx_ground,posx_ground,COLOR_TRUTH);
            cv::line(*output,prev_posy_ground,posy_ground,COLOR_TRUTH);
        }

        prev_posx_measure = posx_measure;
        prev_posy_measure = posy_measure; 
        prev_posx_ground = posx_ground;
        prev_posy_ground = posy_ground; 
        prev_posx_kalman = posx_kalman;
        prev_posy_kalman = posy_kalman; 
    }
}

void displayError(cv::Mat *output){
    size_t N = vecMeasures.size(); 

    // ERROR KALMAN
    cv::Point prev_pos;

    double mean_error_kalman = 0;
    for(size_t i=0; i < N ; i++)
    {
        cv::Point pos;
        pos.x = i* WIDTH / N;
        double error = abs(vecEstimated.at(i).x - vecGround.at(i).x);
        mean_error_kalman += error;
        // DEBUG_MSG("error-kalman  " << i << ": " << error);
        // error *= 10;
        pos.y = HEIGHT-(error-YMIN)*HEIGHT/(YMAX-YMIN);
            // cv::circle(output,pos,3,COLOR_KALMAN,-1,0);
        if(i>0)
            cv::line(*output,prev_pos,pos,COLOR_KALMAN);
        prev_pos=pos;
        // DEBUG_MSG("<TIP tip_position.x=\"" << vecGround.at(i).x << "\" tip_position.y=\"" << vecGround.at(i).y << "\" index_frame=\"" << i << "\"/>");
    }

    // ERROR MEASURE
    double mean_error_measure = 0;
    for(size_t i=0; i < N ; i++)
    {
        cv::Point pos;
        pos.x = i* WIDTH / N;
        double error = abs(vecMeasures.at(i).x - vecGround.at(i).x);
        mean_error_measure += error;
        // DEBUG_MSG("error satep " << i << ": " << error);
        // error += 10;
        pos.y = HEIGHT-(error-YMIN)*HEIGHT/(YMAX-YMIN);
            // cv::circle(*output,pos,3,COLOR_KALMAN,-1,0);
        if(i>0)
            cv::line(*output,prev_pos,pos,COLOR_MEASURE);
        prev_pos=pos;
    }

    mean_error_kalman /= N;
    mean_error_measure /= N;
   
    DEBUG_MSG("mean_error_measure = " << mean_error_measure);
    DEBUG_MSG("mean_error_kalman = " << mean_error_kalman);
}

void readMeasureFile(std::string filename){
    xmlDoc.load_file(filename.c_str());

    pugi::xml_node tipNode = xmlDoc.child("Statistics");

            // FIND MAX NB FRAME
    int nbFrame = 0;
    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        nbFrame = nbFrame > nodeTipInfo.attribute("index_frame").as_int() ? nbFrame : nodeTipInfo.attribute("index_frame").as_int();
    }

    for (int i = 0; i < nbFrame+1; ++i)
    {
        vecMeasures.push_back(cv::Point(0,0));
    }

    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        int node_index_frame = nodeTipInfo.attribute("index_frame").as_int();
        int node_posX = nodeTipInfo.attribute("tip_position.x").as_int();
        int node_posY = nodeTipInfo.attribute("tip_position.y").as_int();
        cv::Point node_position(node_posX,node_posY+120);

        vecMeasures.at(node_index_frame) = node_position;
    }

    // // Smooth measurement
    // size_t N = vecMeasures.size();
    // for(size_t i=2; i < N-2 ; i++)
    // {
    //     vecMeasures.at(i) = vecMeasures.at(i-2) + 2 * vecMeasures.at(i-1) + 3 * vecMeasures.at(i) + 2 * vecMeasures.at(i+1) + vecMeasures.at(i+2);
    //     vecMeasures.at(i) = vecMeasures.at(i) * (1. / 9.);
    // }

    // DEBUG_VEC(vecMeasures);
}

void readGroundTruth(std::string filename){
    xmlDoc.load_file(filename.c_str());

    pugi::xml_node tipNode = xmlDoc.child("tip_positions");

            // FIND MAX NB FRAME
    int nbFrame = 0;
    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        nbFrame = nbFrame > nodeTipInfo.attribute("index_frame").as_int() ? nbFrame : nodeTipInfo.attribute("index_frame").as_int();
    }

    for (int i = 0; i < nbFrame+1; ++i)
    {
        vecGround.push_back(cv::Point(0,0));
    }

    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        int node_index_frame = nodeTipInfo.attribute("index_frame").as_int();
        int node_posX = nodeTipInfo.attribute("tip_position.x").as_int();
        int node_posY = nodeTipInfo.attribute("tip_position.y").as_int();
        cv::Point node_position(node_posX,node_posY);

        vecGround.at(node_index_frame) = node_position;
    }

        // DEBUG_VEC(vecGround);

        // Smooth ground
    size_t N = vecMeasures.size();
    for(size_t i=2; i < N-2 ; i++)
    {
        vecGround.at(i) = vecGround.at(i-2) + 2 * vecGround.at(i-1) + 3 * vecGround.at(i) + 2 * vecGround.at(i+1) + vecGround.at(i+2);
        vecGround.at(i) = vecGround.at(i) * (1. / 9.);
    }
}