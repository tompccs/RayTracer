#ifndef __RayTracer2_0__Box__
#define __RayTracer2_0__Box__

#include <iostream>
#include "Sheet.h"
#include <vector>
#include "functions.h"


using namespace std;

class Box : public Sheet{
    Sheet base, top, left, right, front, back;
    double height, refractiveindex;
    Functions* calc = new Functions;
    
public:
    
    Box();
    
    Box(Sheet* sheet, const double& Height);
    
    Box(const Box& box);
    
    ~Box();
    
    Box& Set(Sheet* sheet, const double& h);
    
    double GetVolume();
    
    double& GetRefractiveIndex();
    
    void SetRefractiveIndex(const double& n);
    
    Box& SetSides();
    
    Sheet& GetBase();
    
    Sheet& GetTop();
    
    Sheet& GetLeft();
    
    Sheet& GetRight();
    
    Sheet& GetBack();
    
    Sheet& GetFront();
    
    bool PointinBox(Photon* photon);
    
    double GetInterfaceDistance(Photon* photon);
    
    Point3D GetInterfacePoint(Photon* photon);
    
    Sheet& GetInterfaceSheet(Photon* photon);
    
    
};

#endif
