#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <stdlib.h> 
#include <sstream>

#define DEBUG
#ifdef DEBUG
#define DEBUG_MSG(str) std::cout << str << std::endl;
#else
#define DEBUG_MSG(str) ;
#endif

int N=0;
std::vector<double> x;
std::vector<double> y;
std::vector<double> vx;
std::vector<double> vy;
std::vector<double> ax;
std::vector<double> ay;
std::vector< std::vector<double> > X;

// ---------------- main ---------------- 
int main(int argc, char **argv)
{
	// READ MEASURE
	std::cin >> N;
	for(int i=0; i < N ; i++)
	{
		double tmp;
		std::cin >> tmp;
		x.push_back(tmp);
		if(i>0)
			vx.push_back(x.at(i) - x.at(i-1));
		if(i>1)
			ax.push_back(vx.at(i-1) - vx.at(i-2));
	}
	for(int i=0; i < N ; i++)
	{
		double tmp;
		std::cin >> tmp;
		y.push_back(tmp);
		if(i>0)
			vy.push_back(y.at(i) - y.at(i-1));
		if(i>1)
			ay.push_back(vy.at(i-1) - vy.at(i-2));
	}
	x.erase(x.begin());
	x.erase(x.begin());
	y.erase(y.begin());
	y.erase(y.begin());
	vx.erase(vx.begin());
	vy.erase(vy.begin());
	N = N-2;


	X.push_back(x);
	X.push_back(y);
	X.push_back(vx);
	X.push_back(vy);
	X.push_back(ax);
	X.push_back(ay);

	for(int i=0; i < N ; i++)
	{
		DEBUG_MSG("x" << i << "=" << x.at(i) << ", y" << i << "=" << y.at(i) << ", vx" << i << "=" << vx.at(i)  << ", vy" << i << "=" << vy.at(i));
	}

	// Evaluate mean
	double* mean;
	mean = (double*) malloc(sizeof(double)*6);
	for (int i = 0; i < 6; ++i)
	{
		double tmp = 0;
		for (int k = 0; k < N; ++k)
		{
			tmp += X.at(i).at(k);
		}
		tmp /= N;
		mean[i] = tmp;
	}

	// Evaluate cov
	double** cov;
	cov = (double**) malloc(sizeof(double)*6);
	for (int i = 0; i < 6; ++i)
	{
		cov[i] = (double*) malloc(sizeof(double)*6);
	}

	for(int i=0; i < 6 ; i++)
	{
		for (int j = 0; j < 6; ++j)
		{
			double tmp = 0;
			for (int k = 0; k < N; ++k)
			{
				tmp += (X.at(i).at(k)-mean[i]) * (X.at(j).at(k)-mean[j]);
			}
			tmp /= N;
			cov[i][j] = tmp;
			DEBUG_MSG("Cov_" << i << "," << j << "=" << tmp);
		}
	}

	std::stringstream covDisplay;
	for (int i = 0; i < 6; ++i)
	{
		covDisplay << "[ ";
		for (int j = 0; j < 6; ++j)
		{
			covDisplay << cov[i][j];
			if(j != 5)
				covDisplay << ", ";
		}
		covDisplay << "]\n";
	}
	DEBUG_MSG(covDisplay.str());


	}

