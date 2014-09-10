#ifndef __RayTracer__MATLABPrint__
#define __RayTracer__MATLABPrint__

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "Test.h"
#include "Material.h"
#include "MultipleObjects.h"


class MATLABPrint{
    Test* print = new Test;
    vector<Point3D> absorbpoints;
    
    
public:
    void PrintLSCs(MultipleObjects* mo);
    string CentrePoint(Material* box);
    string Size(Material* box);
    string PositionPrint(Photon* photon);
    void DyeAbsorbPrint(vector<Point3D> points);
    void PhotonPathPrint(vector<vector<Point3D>>& paths);

};

#endif