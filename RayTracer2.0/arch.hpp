#ifndef arch_hpp
#define arch_hpp

#include <stdio.h>
#include <math.h>
#include "tube.hpp"

class arch{
    tube Tube;
    double startangle;
    double endangle;
    
public:
    void SetTube(tube t);
    void SetStart(double& s);
    void SetEnd(double& e);
    
    tube& GetTube();
    double& GetStart();
    double& GetEnd();
    
    int FindIntersection(Photon& p, bool debug, combined values);
    
    double CheckAngle(Point3D& p);
};


#endif