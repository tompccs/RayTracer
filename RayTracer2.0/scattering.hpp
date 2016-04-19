#ifndef scattering_hpp
#define scattering_hpp

#include <stdio.h>
#include "probdistribution.hpp"
#include "Functions.h"
#include "Vector3D.h"


class scattering{
    probdistribution front;
    probdistribution back;
    vector <double> frontbackdistribution;
    List data;
    Functions num;
    
    double theta;
    double phi;
    
public:
    void GetDistributions(int radius, int lambda, bool hot);
    bool RollHalfSpace();
    void RollCoordinates();
    Vector3D Rotated(Vector3D mom, double theta, double phi);
    double& GetTheta();
    double& GetPhi();
};

#endif