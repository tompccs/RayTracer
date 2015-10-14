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
    bool LineOnEllipseIntersection(Photon& photon);
    combined photonellipseintersect(Photon& photon);
    Point3D nextpoint(Photon& photon, combined& checker);
    Point3D nextpoint3D(Photon& photon);
    void SetCentre(Point3D& centre);
    void SetA(double A);
    void SetB(double B);
    Point3D GetCentre();
    double GetA();
    double GetB();
    
};

#endif