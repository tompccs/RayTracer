#ifndef arc_hpp
#define arc_hpp

#include <stdio.h>
#include "ellipse.hpp"
#include "FresnelJackson.h"

class arc: public ellipse{
    ellipse e;
    double startangle;
    double endangle;
    combined storage;
    Test reader;
    
//XThetaAngles Range between 0 and Pi
//YThetaAngles Range between -Pi/2 and Pi/2
    
public:
    arc();
    arc(ellipse& elps, double start, double end);
    arc(Point3D& centre, double a, double b, double start, double end);
    bool pointonarc(Point3D& point);
    bool photonarcintersect(Photon& photon);
    combined GetStorage();
    Point3D GetNextPoint(Photon& photon);
    
    void setstartangle(double startangle);
    void setendangle(double endangle);
    
    double& getstartangle();
    double& getendangle();
    
    
    double IntersectDistance(Photon& photon);
    double distancetocentre(Point3D& point);
    
    void SetStorageCheck(bool b);
    void SetStoragePoint1(Point3D p);
    void SetStoragePoint2(Point3D p);
    Vector3D InsideNormalVector(Point3D& p);
    Vector3D OutsideNormalVector(Point3D& p);
    bool IntersectionConcave(Photon& photon);
    Vector3D GetNormalVector(Photon& photon);
    
    
    
};


#endif