#include <iostream>
#include <vector>
#include <string>
#include <ctime>

// #define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#else
#define DEBUG_MSG(str) ;
#endif


#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "pugixml.hpp"

struct Pair
{
    double x;
    double y;
    Pair(double x, double y) : x(x), y(y) {};
    Pair() : x(0), y(0) {};
};

struct Statistics
{
    Pair mean;
    Pair var;
    int indexFrame;
    Statistics(int indexFrame) : indexFrame(indexFrame) {};
    void setMean(Pair pair) { mean = pair; };
    void setVar(Pair pair) { var = pair; };
};

struct TipInfo
{
    cv::Point pos;
    int indexFrame;
    TipInfo(cv::Point pos, int indexFrame) : pos(pos), indexFrame(indexFrame) {};
};

struct TipMeasure
{
    int indexFrame;
    std::vector<cv::Point> positions;
    TipMeasure(int indexFrame) : indexFrame(indexFrame) {};
    void addMeasure(cv::Point measure) { positions.push_back(measure);};
};

// FUNCTION
void xmlFillGroundTruth();
void xmlFillMeasures();
void printVec(std::vector<Statistics> a_vec);
void displayMeanDiff();

// PARAMETERS
std::string filenameGroundTruth;
std::vector<TipInfo> mTipGroundTruth;
pugi::xml_document xmlDocGroundTruth;

std::string filenameMeasures;
std::vector<TipMeasure> mTipMeasures;
std::vector<Statistics> mStatisticsMeasures;
pugi::xml_document xmlDocMeasures;


// ---------------- main ---------------- 
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        DEBUG_MSG("Usage : GenerateStat fileWithGroundTruth fileWithMeasure");
        return 0;
    }
    // READ GROUND_TRUTH
    DEBUG_MSG("READING GROUND_TRUTH");
    filenameGroundTruth = argv[1];
    xmlFillGroundTruth();

    // READ MEASURE
    DEBUG_MSG("READING MEASURE");
    filenameMeasures = argv[2];
    xmlFillMeasures();

    // Init vec
    for(std::vector<TipMeasure>::iterator tipMeasure = mTipMeasures.begin(); tipMeasure != mTipMeasures.end(); ++tipMeasure)
    {
        mStatisticsMeasures.push_back(Statistics(tipMeasure->indexFrame));
    }

    // Compute mean of Tip measures
    size_t index = 0;
    for (std::vector<TipMeasure>::iterator tipMeasure = mTipMeasures.begin(); tipMeasure != mTipMeasures.end(); ++tipMeasure)
    {
        Pair mean;
        for (std::vector<cv::Point>::iterator it = tipMeasure->positions.begin(); it != tipMeasure->positions.end(); ++it)
        {
            mean.x += it->x;
            mean.y += it->y;
        }
        mean.x /= (double)tipMeasure->positions.size();
        mean.y /= (double)tipMeasure->positions.size();
        mStatisticsMeasures.at(index).setMean(mean);
        index++;
    }

    // Compute var of Tip measures
    index = 0;
    for (std::vector<TipMeasure>::iterator tipMeasure = mTipMeasures.begin(); tipMeasure != mTipMeasures.end(); ++tipMeasure)
    {
        Pair var;
        for (std::vector<cv::Point>::iterator it = tipMeasure->positions.begin(); it != tipMeasure->positions.end(); ++it)
        {
            var.x += pow((it->x-mStatisticsMeasures.at(index).mean.x),2);
            var.y += pow((it->y-mStatisticsMeasures.at(index).mean.y),2);
        }
        var.x /= (double)tipMeasure->positions.size();
        var.y /= (double)tipMeasure->positions.size();
        mStatisticsMeasures.at(index).setVar(var);
        index++;
    }

    // printVec(mStatisticsMeasures);

    displayMeanDiff();
  
}

// ---------------- xmlFillGroundTruth ---------------- 
void xmlFillGroundTruth()
{
    xmlDocGroundTruth.load_file(filenameGroundTruth.c_str());

    pugi::xml_node tipNode = xmlDocGroundTruth.child("tip_positions");

    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        int node_index_frame = nodeTipInfo.attribute("index_frame").as_int();
        int node_posX = nodeTipInfo.attribute("tip_position.x").as_int();
        int node_posY = nodeTipInfo.attribute("tip_position.y").as_int();
        TipInfo tipInfo(cv::Point(node_posX,node_posY),node_index_frame);
        mTipGroundTruth.push_back(tipInfo);    
    }
}

// ---------------- xmlFillMeasures ---------------- 
void xmlFillMeasures()
{
    xmlDocMeasures.load_file(filenameMeasures.c_str());

    pugi::xml_node tipNode = xmlDocMeasures.child("Statistics");

    for (pugi::xml_node nodeTipInfo = tipNode.child("TIP"); nodeTipInfo; nodeTipInfo = nodeTipInfo.next_sibling("TIP"))
    {
        int node_index_frame = nodeTipInfo.attribute("index_frame").as_int();
        int node_posX = nodeTipInfo.attribute("tip_position.x").as_int();
        int node_posY = nodeTipInfo.attribute("tip_position.y").as_int();
        cv::Point node_position(node_posX,node_posY+120);

        bool hasBeenInserted = false;
        for(size_t i = 0; i < mTipMeasures.size() ; i++)
        {
            TipMeasure* current = &(mTipMeasures.at(i));
            if(current->indexFrame == node_index_frame)
            {
                current->addMeasure(node_position);
                hasBeenInserted = true;
                break;
            }
            else if (current->indexFrame > node_index_frame)
            {
                TipMeasure newTipMeasure(node_index_frame);
                newTipMeasure.addMeasure(node_position);
                mTipMeasures.insert(mTipMeasures.begin() + i, newTipMeasure);
                hasBeenInserted = true;
                break;
            }
        }

        if(!hasBeenInserted)
        {
            TipMeasure newTipMeasure(node_index_frame);
            newTipMeasure.addMeasure(node_position);
            mTipMeasures.push_back(newTipMeasure);
        }
    }
}

// ---------------- printVec ---------------- 
void printVec(std::vector<Statistics> a_vec)
{
#ifdef DEBUG
    size_t index = 0;
    for (std::vector<Statistics>::iterator it = a_vec.begin(); it != a_vec.end(); ++it)
    {
        std::stringstream msg;
        msg << "Frame " << it->indexFrame;
        msg << ": Mean: (" << it->mean.x << "," << it->mean.y << ")";
        msg << ", Var: (" << it->var.x << "," << it->var.y << ")";
        
        TipMeasure current = mTipMeasures.at(index);

        DEBUG_MSG(msg.str());
        msg.str("");
        msg << "---> Measures being: ";

        for (std::vector<cv::Point>::iterator it = current.positions.begin(); it != current.positions.end(); ++it)
        {
            msg << *it;
            if(it != current.positions.end()-1)
                msg << ", ";
        }
        DEBUG_MSG(msg.str());
    }
#endif
}

// ---------------- displayMeanDiff ---------------- 
void displayMeanDiff()
{ 
 // Compare mean to ground truth
    size_t index = 0;
    for(std::vector<Statistics>::iterator currentStat = mStatisticsMeasures.begin(); currentStat != mStatisticsMeasures.end() ; ++currentStat)
    {
        double meanx = currentStat->mean.x;
        double meany = currentStat->mean.y;
        cv::Point ground = mTipGroundTruth.at(index).pos;
        std::cout << index << " " << meanx << " " << meany << " " << ground.x << " " << ground.y <<  std::endl; 
        index++;
    }
}