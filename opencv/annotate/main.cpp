#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#else
#define DEBUG_MSG(str) ;
#endif


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "fldatahandler.h"
#include "pugixml.hpp"

// FUNCTION
void showPic();
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
void updateTip(int x,int y);
void removeTip();

void xmlInit(std::string filename);
void xmlAddKey(int x, int y, int a_indexFrame);
void xmlRemoveLastKey();
int xmlGetLastProcessImage();

// GLOBAL PARAMETERS
int mHeight, mWidth, mIndexCurrentFrame;
bool mInsertMode;
FlDataHandler* mFlDataHandler;
cv::Mat* mImage;
std::string mXMLFilename;
static pugi::xml_document xmlDoc;
static pugi::xml_parse_result xmlResult;
std::vector<cv::Point> mTipPositions;

// ---------------- main ---------------- 
int main(int argc, char **argv)
{
    if(argc < 3)
    {
        std::cout << "Usage:  ./Annotate fileToAnnotate fileToWrite" << std::endl;
        return 0; 
    }

    // PARSING FILE
    DEBUG_MSG("Parsing file");
    mFlDataHandler = new FlDataHandler(argv[1]);
    DEBUG_MSG("File parsed");

    // Initialization
    DEBUG_MSG("Initialization");
    mHeight = mFlDataHandler->h;
    mWidth= mFlDataHandler->w;
    mIndexCurrentFrame = 0;
    mXMLFilename = argv[2];
    xmlInit(mXMLFilename);;
    mInsertMode = false;

    mIndexCurrentFrame = xmlGetLastProcessImage();

    DEBUG_MSG("Starting from frame " << mIndexCurrentFrame);

    // Show image
    cv::namedWindow("Annotate",1);
    cv::setMouseCallback("Annotate", CallBackFunc, NULL);
    showPic();
    cv::waitKey(0);
}

void showTips()
{
    for (std::vector<cv::Point>::iterator i = mTipPositions.begin(); i != mTipPositions.end(); ++i)
    {
       cv::circle(*mImage,*i,3,cv::Scalar(100,100,0),-1); 
    }
}

void showPic()
{
    mImage = new cv::Mat(cv::Mat::zeros(mHeight,mWidth, CV_32FC1));

    for (int x = 0; x < mHeight; ++x) {
        for (int y = 0; y < mWidth; ++y) {
            mImage->at<float>(x,y) = mFlDataHandler->allPictures[mIndexCurrentFrame][x][mWidth-y];
        }
    }

    if(!mInsertMode)
        showTips();

    cv::cvtColor(*mImage, *mImage, CV_GRAY2RGB);
    cv::imshow("Annotate",*mImage); 
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == cv::EVENT_LBUTTONDOWN )
    {
        DEBUG_MSG("Left button of the mouse is clicked - position (" << x << ", " << y << ")");
        if(mInsertMode)
        {
            mInsertMode = !mInsertMode;
            updateTip(x,y);
            showPic();
        }
        else
        {
            mInsertMode = !mInsertMode;
            showPic();
        }
    }

    if  ( event == cv::EVENT_RBUTTONDOWN )
    {
        mInsertMode = false;
        DEBUG_MSG("Right button of the mouse is clicked - position (" << x << ", " << y << ")");
        removeTip();
        showPic();
    }
    // else if  ( event == cv::EVENT_RBUTTONDOWN )
    // {
    //     std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
    // }
    // else if  ( event == cv::EVENT_MBUTTONDOWN )
    // {
    //     std::cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
    // }
    // else if ( event == cv::EVENT_MOUSEMOVE )
    // {
    //     std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << std::endl;
    // }
}

void updateTip(int x,int y)
{
    DEBUG_MSG("Adding tip position in " << x << "," << y);
    mTipPositions.push_back(cv::Point(x,y));
    xmlAddKey(x,y,mIndexCurrentFrame);
    mIndexCurrentFrame += mIndexCurrentFrame < mFlDataHandler->nframes-1 ? 1 : 0;
}

void removeTip()
{
    DEBUG_MSG("Removed last tip insertion");
    mTipPositions.pop_back();
    xmlRemoveLastKey();
    mIndexCurrentFrame -= mIndexCurrentFrame > 0 ? 1 : 0;
}

void xmlInit(std::string filename)
{
    std::ifstream infile(filename.c_str());
    
    if(!infile.good())
    {
        std::ofstream file;
        file.open(filename.c_str());

        // INTRO
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
        file << "<tip_positions>" << std::endl;

        // SOME INFO
        file << "<file>" << mFlDataHandler->mFilename << "</file>" << std::endl; 

        time_t now = time(0);
        char* dt = ctime(&now);
        file << "<time>" << dt << "</time>" << std::endl;

        // CLOSURE
        file << "</tip_positions>";
        file.close();
    }

    // INIT pugixml
    xmlResult = xmlDoc.load_file(filename.c_str());
}

void xmlAddKey(int x, int y, int a_indexFrame)
{
    pugi::xml_node tipNode = xmlDoc.child("tip_positions");
    pugi::xml_node nodeTipInfo = tipNode.append_child("TIP");

    nodeTipInfo.append_attribute("tip_position.x") = x;
    nodeTipInfo.append_attribute("tip_position.y") = y;
    nodeTipInfo.append_attribute("index_frame") = a_indexFrame;

    xmlDoc.save_file(mXMLFilename.c_str());
}

void xmlRemoveLastKey()
{
    pugi::xml_node tipNode = xmlDoc.child("tip_positions");

    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        int node_index_frame = nodeTipInfo.attribute("index_frame").as_int();
        if(node_index_frame = mIndexCurrentFrame)
            tipNode.remove_child(nodeTipInfo);
    }
}

int xmlGetLastProcessImage()
{
    pugi::xml_node tipNode = xmlDoc.child("tip_positions");

    int result = 0;

    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        int node_index_frame = nodeTipInfo.attribute("index_frame").as_int();
        result = node_index_frame > result ? node_index_frame : result;

        int node_posX = nodeTipInfo.attribute("tip_position.x").as_int();
        int node_posY = nodeTipInfo.attribute("tip_position.y").as_int();
        mTipPositions.push_back(cv::Point(node_posX,node_posY));
    }

    return result;
}