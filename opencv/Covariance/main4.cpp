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
std::vector< std::vector<double> > error;

// ---------------- main ---------------- 
int main(int argc, char **argv)
{
	// READ measures
	int N;
	std::cin >> N;
	for(int i=0; i < N ; i++)
	{
		double ex;
		std::cin >> ex;
		double ey;
		std::cin >> ey;
		std::vector<double> tmp;
		tmp.push_back(ex);
		tmp.push_back(ey);
		error.push_back(tmp);
	}

	double* mean;
	mean = (double*) malloc(sizeof(double)*2);
	for(int i=0; i < 2 ; i++)
	{
		double tmp = 0;
		for (int k = 0; k < N; ++k)
		{
			tmp += error.at(k).at(i);
		}
		mean[i] = tmp / N;
		DEBUG_MSG("mean[" << i << "] = " << mean[i]);
	}

	// double** cov;
	// cov = (double**) malloc(sizeof(double*)*6);

	// for(int i=0; i < 6 ; i++)
	// {
	// 	cov[i] = (double*) malloc(sizeof(double)*6);
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		double tmp = 0;
	// 		for (int k = 0; k < N; ++k)
	// 		{
	// 			tmp += (points.at(k).at(i) - mean[i])*(points.at(k).at(j) - mean[j]);
	// 		}
	// 		tmp /= N;
	// 		cov[i][j] = tmp;
	// 	}
	// }

	// for (int i = 0; i < 6; ++i)
	// {
	// 	std::stringstream msg;
	// 	// msg << "";
	// 	for (int j = 0; j < 6; ++j)
	// 	{
	// 		msg << cov[i][j];
	// 		if(j!=5)
	// 			msg << ",";
	// 	}
	// 	msg << "\\\\";
	// 	DEBUG_MSG(msg.str());
	// }

}

