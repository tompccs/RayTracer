#ifndef arc_hpp
#define arc_hpp

#include <stdio.h>
#include "ellipse.hpp"

class arc: public ellipse{
    ellipse e;
    double startangle;
    double endangle;
    
//XThetaAngles Range between 0 and Pi
//YThetaAngles Range between -Pi/2 and Pi/2
//
    
public:
    arc();
    arc(ellipse& elps, double start, double end);
    arc(Point3D& centre, double a, double b, double start, double end);
    bool pointonarc(Point3D& point);
    bool photonarcintersect(Photon& photon);
};


#endif