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

#define PRINT
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

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cout << "Input: measure, ground truth" << std::endl;
        return 0;
    }
            // READ MEASUREMENTS
    std::string filename1 = argv[1];
    std::string filename2 = argv[2];
    readMeasureFile(filename1);
    int N = vecMeasures.size();
    DEBUG_MSG("Measures read");
    readGroundTruth(filename2);
    DEBUG_MSG("Ground truth read");

    // // Create kalman
    cv::Mat A = (cv::Mat_<double>(6,6) << 1, 0, 1, 0, 0.5, 0,
        0, 1, 0, 1, 0, 0.5,
        0, 0, 1, 0, 1, 0,
        0, 0, 0, 1, 0, 1,
        0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1 );

    cv::Mat B = (cv::Mat_<double>(6,1) << 1, 0, 0, 0, 0, 0);

    cv::Mat Q= (cv::Mat_<double>(6,6) <<
        1e-3,1e-3,1e-3,1e-3,1e-3,1e-3,
        1e-3,1e-3,1e-3,1e-3,1e-3,1e-3,
        1e-3,1e-3,1e-3,1e-3,1e-3,1e-3,
        1e-3,1e-3,1e-3,1e-3,1e-3,1e-3,
        1e-3,1e-3,1e-3,1e-3,1e-3,1e-3,
        1e-3,1e-3,1e-3,1e-3,1e-3,1e-3);

    // for(int i=0; i < 6 ; i++)
    // {
    //     for(int j= 0 ; j < 6 ; j++)
    //     {
    //         Q.at<double>(i,j) += ((double)(rand() % 10) - 5)/100.0;
    //     }
    // }
            // 6725.2378976052 ,3.8299368345    ,6728.5345743117     ,5.4833248793        ,6725.7565488606     ,0.047147905,
        // 2.190275318     ,6728.6435283637 ,9.552230991         ,6728.5197591838     ,1.2085585436        ,6727.204275989,
        // 6726.7958094282 ,0.4336166685    ,13001.5730500468    ,0.440078692         ,19605.013554045     ,7.1279468224,
        // 2.5002462487    ,6724.7756043784 ,8.368421332         ,13005.9786789273    ,3.3835149975        ,19601.2418044056,
        // 6724.624768096  ,5.4394991347    ,19609.8576591024    ,4.9374689115        ,40009.9219300365    ,6.474043522,
        // 4.1557390173    ,6724.1056129485 ,1.695682304         ,19604.1920411354    ,5.9087028145        ,40007.0959469955);

    cv::Mat C = (cv::Mat_<double>(2,6) << 
        1, 0, 0, 0, 0, 0, 
        0, 1, 0, 0, 0, 0);

    cv::Mat R = (cv::Mat_<double>(2,2) << 
        10, 0,
        0, 10);


    kalman = new Kalman(A,B,Q,C,R);

    cv::Mat mu0 = (cv::Mat_<double>(6,1) << vecGround.begin()->x,vecGround.begin()->y,0,0,0,0);
    cv::Mat Sigma0 = (cv::Mat_<double>(6,6) <<
        44.91527728182324, 29.01907585420759, 5.227328694788755, 4.829069020350386, 0.3456378542995343, 0.3414802562278842,
        29.01907584281928, 44.91527723908357, 4.829069011116202, 5.227328683974491, 0.3414802554128484, 0.3456378534598457,
        5.227328697840449, 4.829069008632975, 1.080450204820537, 1.067051349588245, 0.09986170623990445, 0.09970397119904237,
        4.829069013281678, 5.22732869047434, 1.067051349387744, 1.080450204014527, 0.09970397116222871, 0.09986170619364397,
        0.3456378542922439, 0.341480255428902, 0.09986170623718751, 0.09970397116803979, 0.0136366084698522, 0.01363462620177318,
        0.341480255914507, 0.3456378537644058, 0.09970397118575476, 0.09986170620382954, 0.01363462620152601, 0.01363660846880824);


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
        // kalman->printInfo();
        index++; 
    }

    // Display
    output = cv::Mat::zeros(WIDTH,HEIGHT,CV_8UC3);
    for(int i = 0; i < WIDTH; i ++)
    {
        for(int j=0 ; j < HEIGHT ; j++)
        {
            cv::circle(output,cv::Point(i,j),3,cv::Scalar(255,255,255),-1,0);
        }
    }

    cv::Point prev_pos;

    // Measurement
    for(size_t i=0; i < N ; i++)
    {
        cv::Point pos;
        pos.x = i* WIDTH / N;
        pos.y = HEIGHT-(vecMeasures.at(i).x-YMIN)*HEIGHT/(YMAX-YMIN);
        // cv::circle(output,pos,3,COLOR_MEASURE,-1,0);
        if(i>0)
            cv::line(output,prev_pos,pos,COLOR_MEASURE);
        prev_pos = pos;
    }

    // Estimate
    PRINT_MSG("var Kalman= ["); // ]
    for(size_t i=0; i < N ; i++)
    {
        cv::Point pos;
        pos.x = i* WIDTH / N;
        pos.y = HEIGHT-(vecEstimated.at(i).x-YMIN)*HEIGHT/(YMAX-YMIN);
        // cv::circle(output,pos,3,COLOR_KALMAN,-1,0);
        if(i>0)
            cv::line(output,prev_pos,pos,COLOR_KALMAN);
        prev_pos = pos;
        if(i<N-1){
            PRINT_MSG("{Frame:"<<i<<",Kalmanx:"<<vecEstimated.at(i).x<<",Kalmany:"<<vecEstimated.at(i).y<<"},");
        }
        else{
            PRINT_MSG("{Frame:"<<i<<",Kalmanx:"<<vecEstimated.at(i).x<<",Kalmany:"<<vecEstimated.at(i).y<<"}];");
        }
    }

    // Ground truth
    for(size_t i=0; i < N ; i++)
    {
        cv::Point pos;
        pos.x = i* WIDTH / N;
        pos.y = HEIGHT-(vecGround.at(i).x-YMIN)*HEIGHT/(YMAX-YMIN);
        // cv::circle(output,pos,3,COLOR_TRUTH,-1,0);
        if(i>0)
            cv::line(output,prev_pos,pos,COLOR_TRUTH);
        prev_pos=pos;
        // DEBUG_MSG(vecGround.at(i).x << "," << vecGround.at(i).y);
    }

    // ERROR KALMAN
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
            cv::line(output,prev_pos,pos,COLOR_KALMAN);
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
            // cv::circle(output,pos,3,COLOR_KALMAN,-1,0);
        if(i>0)
            cv::line(output,prev_pos,pos,COLOR_MEASURE);
        prev_pos=pos;
    }

    mean_error_kalman /= N;
    mean_error_measure /= N;

    DEBUG_MSG("mean_error_measure = " << mean_error_measure);
    DEBUG_MSG("mean_error_kalman = " << mean_error_kalman);
    imshow("ouput",output);
    cv::waitKey(0);

}


void readMeasureFile(std::string filename)
{
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

void readGroundTruth(std::string filename)
{
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