#include "algorithmroi.h"

AlgorithmROI::AlgorithmROI() :
    mROIComputed(false)
{
    C = (cv::Mat_<double>(3,3) <<
                 0.087, -0.0033, -1.00,
                -29.7558, 0.9433, -0.0034
                -0.7053, 0.0132, -0.0087);
}

void AlgorithmROI::applyAlgorithm(FlGPSHandler *a_flGPSHandler1, FlGPSHandler *a_flGPSHandler2, int a_indexFrame)
{
    // FILL SP
    SP = (cv::Mat_<double>(3,3) <<
          a_flGPSHandler1->gps_s[a_indexFrame][0][0],
          a_flGPSHandler1->gps_s[a_indexFrame][0][1],
          a_flGPSHandler1->gps_s[a_indexFrame][0][2],
          a_flGPSHandler1->gps_s[a_indexFrame][1][0],
          a_flGPSHandler1->gps_s[a_indexFrame][1][1],
          a_flGPSHandler1->gps_s[a_indexFrame][1][2],
          a_flGPSHandler1->gps_s[a_indexFrame][2][0],
          a_flGPSHandler1->gps_s[a_indexFrame][2][1],
          a_flGPSHandler1->gps_s[a_indexFrame][2][2]
          );
    // FILL SN
    SN = (cv::Mat_<double>(3,3) <<
          a_flGPSHandler2->gps_s[a_indexFrame][0][0],
          a_flGPSHandler2->gps_s[a_indexFrame][0][1],
          a_flGPSHandler2->gps_s[a_indexFrame][0][2],
          a_flGPSHandler2->gps_s[a_indexFrame][1][0],
          a_flGPSHandler2->gps_s[a_indexFrame][1][1],
          a_flGPSHandler2->gps_s[a_indexFrame][1][2],
          a_flGPSHandler2->gps_s[a_indexFrame][2][0],
          a_flGPSHandler2->gps_s[a_indexFrame][2][1],
          a_flGPSHandler2->gps_s[a_indexFrame][2][2]
          );
    // FILL ORN
    ORN = (cv::Mat_<double>(3,1) <<
           a_flGPSHandler2->gps_posx[a_indexFrame],
           a_flGPSHandler2->gps_posy[a_indexFrame],
           a_flGPSHandler2->gps_posz[a_indexFrame]);

    // Calcul OIN
    // OIN = C-1 SP ORN
    OIN = C.inv() * SP * SN * ORN;

    DEBUG_MSG("OIN = " << OIN);

    // Calcul XIN
    // XIN = C-1 SP SN [1,0,0];

    cv::Mat vecUnitx = (cv::Mat_<double>(3,1) <<
                        1,
                        0,
                        0);

    XIN = C.inv() * SP * SN * vecUnitx;

    DEBUG_MSG("XIN = " << XIN);

    // TEMP

//    std::vector< std::pair<int,int> > pointInROI;
//    for (int i = 0; i < 640; ++i) {
//        for(int j = 0; j < 480 ; ++j){
//            cv::Mat M = (cv::Mat_<double>(3,1) <<
//                         1,
//                         j,
//                         i);
//            float dist = DistToCurve(M,OIN,XIN);
////            DEBUG_MSG("i=" << i << ", j=" << j << " give dist=" << dist);
//            if(dist <= 10){
//                pointInROI.push_back(std::make_pair(i,j));
//            }
//        }
//    }

//    for(std::vector< std::pair<int,int> >::iterator it = pointInROI.begin(); it != pointInROI.end(); it++){
//        DEBUG_MSG("= " << (*it).first << ", j= " << (*it).second);
//    }

}

// ------------ isROIComputed ----------------
bool AlgorithmROI::isROIComputed()
{
    return mROIComputed;
}

// ------------ getROI ----------------
cv::RotatedRect* AlgorithmROI::getROI()
{
    return &mROI;
}

double AlgorithmROI::DistToCurve(cv::Mat M, cv::Mat OIN, cv::Mat XIN){
    // Points M1 and M2 on curve, M0 point in space
    cv::Point3d M0 = (cv::Point3d) M;
    cv::Point3d M1 = cv::Point3d(OIN.at<double>(0,0), OIN.at<double>(1,0), OIN.at<double>(2,0));
    cv::Point3d M2 = cv::Point3d(OIN.at<double>(0,0) + XIN.at<double>(0,0), OIN.at<double>(1,0) + XIN.at<double>(1,0), OIN.at<double>(2,0) + XIN.at<double>(2,0));
    return norm((M0-M1).cross(M0-M2))/norm(M2-M1);
}
