#ifndef __RayTracer__list__
#define __RayTracer__list__

#include <iostream>
#include <vector>
#include <fstream>
#include <cfloat>
#include <math.h>

using namespace std;

class List{
public:
    vector <double> read(string filename);
    vector <double> auconvert(vector <double> &input);
    vector <double> interp1(vector< double > &x, vector< double > &y, vector< double > &x_new );
    vector <double> newxdata(vector<double> &xdata);
    int findNearestNeighbourIndex(double value, vector <double> &x);
    double findFirstIndexLower(double &value, vector<double> &ydata);
    double GetAfromB(double yvalue, vector<double> &xdata, vector<double> &ydata);
    
};

#endif