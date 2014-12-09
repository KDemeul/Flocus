#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <stdlib.h> 
#include <sstream>
#include <math.h>       /* cos */

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#else
#define DEBUG_MSG(str) ;
#endif

#define PI 3.14159265

int N=0;
std::vector<double> x;
std::vector<double> y;
std::vector<double> vx;
std::vector<double> vy;
std::vector<double> ax;
std::vector<double> ay;
std::vector< std::vector<double> > X;

struct customPoint
{
	double x;
	double y;
	double vx;
	double vy;
	double ax;
	double ay;
	customPoint(double x,double y,double vx,double vy,double ax,double ay) : x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay) {};
};

std::string print_X(customPoint a_X)
{
	std::stringstream ss;
	ss << "(" << a_X.x << "," << a_X.y << "," << a_X.vx << "," << a_X.vy << "," << a_X.ax << "," << a_X.ay << ")";
	return ss.str();
}

// ---------------- main ---------------- 
int main(int argc, char **argv)
{
	int rmax = 50;
	DEBUG_MSG("rmax = " << rmax);
	int N = 100000;

	cv::Mat output = cv::Mat::zeros(500,500,CV_8UC3);
	cv::Mat outputv = cv::Mat::zeros(500,500,CV_8UC3);
	cv::Mat outputa = cv::Mat::zeros(500,500,CV_8UC3);

	// Generate N points
	std::vector< std::vector<double> > points;
	std::vector<double> point;
	point.push_back(0);
	point.push_back(0);
	point.push_back(0);
	point.push_back(0);
	point.push_back(0);
	point.push_back(0);

	points.push_back(point);
	points.push_back(point);

	cv::Point prev_point(255,255);
	srand (time(NULL));
	for (int i = 2; i < N+2; ++i) 
	{
		double rho = rand() % rmax;
		double theta = rand() % 360;

		double x = rho * cos (theta * PI / 180.0);
		double y = rho * sin (theta * PI / 180.0);

		double vx = x - points.at(i-1).at(0);
		double vy = y - points.at(i-1).at(1);

		double ax = vx - points.at(i-1).at(2);
		double ay = vy - points.at(i-1).at(3);

		std::vector<double> point;
		point.push_back(x);
		point.push_back(y);
		point.push_back(vx);
		point.push_back(vy);
		point.push_back(ax);
		point.push_back(ay);

		points.push_back(point);

		if(i > 0)
		{
			cv::Point toDisplay = cv::Point(0,0);		
			toDisplay.x = (200.0/30.0)* rho * cos (theta * PI / 180.0) + 250.0;
			toDisplay.y = (200.0/30.0)* rho * sin (theta * PI / 180.0) + 250.0;
			cv::Point vel = cv::Point(0,0);
			// cv::circle(output,toDisplay,1,cv::Scalar(255,0,0),-1,0);
			cv::line(output,prev_point,toDisplay,cv::Scalar(255,0,0,0.1));
			cv::line(output,toDisplay,toDisplay+cv::Point(vx,vy),cv::Scalar(0,0,255,0.1));
			cv::line(output,toDisplay,toDisplay+cv::Point(ax,ay),cv::Scalar(0,255,0,0.1));
			cv::line(outputv,cv::Point(250,250),cv::Point(250,250)+cv::Point(vx,vy),cv::Scalar(0,0,255,0.1));
			cv::line(outputa,cv::Point(250,250),cv::Point(250,250)+cv::Point(ax,ay),cv::Scalar(0,255,0,0.1));
			prev_point = toDisplay;

			// if(i==N+1)
			// {
			imshow("output",output);
			imshow("outputa",outputa);
			imshow("outputv",outputv);
			cv::waitKey(0);
			// }
		}
	}

	double* mean;
	mean = (double*) malloc(sizeof(double)*6);
	for(int i=0; i < 6 ; i++)
	{
		double tmp = 0;
		for (int k = 0; k < N; ++k)
		{
			tmp += points.at(k).at(i);
		}
		mean[i] = tmp / N;
		DEBUG_MSG("mean[" << i << "] = " << mean[i]);
	}

	double** cov;
	cov = (double**) malloc(sizeof(double*)*6);

	for(int i=0; i < 6 ; i++)
	{
		cov[i] = (double*) malloc(sizeof(double)*6);
		for (int j = 0; j < 6; ++j)
		{
			double tmp = 0;
			for (int k = 0; k < N; ++k)
			{
				tmp += (points.at(k).at(i) - mean[i])*(points.at(k).at(j) - mean[j]);
			}
			tmp /= N;
			cov[i][j] = tmp;
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		std::stringstream msg;
		// msg << "";
		for (int j = 0; j < 6; ++j)
		{
			msg << cov[i][j];
			if(j!=5)
				msg << ",";
		}
		msg << "\\\\";
		DEBUG_MSG(msg.str());
	}

}

