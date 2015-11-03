#ifndef __RayTracer2_0__Test__
#define __RayTracer2_0__Test__

#include <iostream>
#include "Box.h"
#include <vector>
#include <fstream>
#include <string>

using namespace std;

class Test{
public:
    
    void PrintPoint(const Point3D& point);
    
    void Print2DPoint(const Point2D& point);
    
    void PrintVector(const Vector3D& vector);
    
    void PrintBool(const bool& b);
    
    void PrintSheet(Sheet& sheet);
    
    void PrintVectorFile(vector<double> list, string filename);
    
    void PhotonPrint(Photon* photon);
    
    void PrintTextVector(vector<string> list, string filename);
    
    string NumberToString(double number);
    
};


#endif