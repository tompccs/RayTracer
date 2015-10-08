#ifndef ellipse_hpp
#define ellipse_hpp

#include <stdio.h>
#include "Box.h"
#include "Point2d.h"
#include "Test.h"
#include "combined.hpp"

class ellipse{
    Point3D centre;
    double a;
    double b;
    
public:
    
    ellipse();
    ellipse(Point3D centre, double a, double b);
    bool pointcheck(Point3D& point);
    bool ellipselineintersectcheck(Photon& photon);
    Point3D firstintersectpoint(Photon& photon);
    combined
    
};

#endif