#ifndef __RayTracer2_0__Test__
#define __RayTracer2_0__Test__

#include <iostream>
#include "Box.h"
#include <vector>
#include <fstream>

using namespace std;

class Test{
public:
    
    void PrintPoint(const Point3D& point);
    
    void PrintVector(const Vector3D& vector);
    
    void PrintBool(const bool& b);
    
    void PrintSheet(Sheet& sheet);
    
    void PrintVectorFile(vector<double> list, string filename);
    
    void PhotonPrint(Photon* photon);
};


#endif