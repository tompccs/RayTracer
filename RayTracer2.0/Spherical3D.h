#ifndef __RayTracer__Spherical3D__
#define __RayTracer__Spherical3D__

#include <stdio.h>
#include "Vector3D.h"

class Spherical3D{
    
    double r;
    double theta;
    double phi;
    
public:
    
    Spherical3D();
    Spherical3D(double r, double theta, double phi);
    Spherical3D(Vector3D& cartesian);
    Vector3D Cartesian();
    Spherical3D& Get();
    void Print();
    
};

#endif