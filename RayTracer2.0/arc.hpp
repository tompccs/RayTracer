#ifndef arc_hpp
#define arc_hpp

#include <stdio.h>
#include "ellipse.hpp"

class arc{
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
    
    
    bool pointonarccheck(Point3D& point, bool debug);

    bool pointonarc(Point3D& point, bool debug);
    bool photonarcintersect(Photon& photon, bool debug);
    combined GetStorage();
    Point3D GetNextPoint(Photon& photon, bool debug);
    
    
    
    void setstartangle(double startangle);
    void setendangle(double endangle);
    
    double& getstartangle();
    double& getendangle();
    
    
    double IntersectDistance(Photon& photon, bool debug);
    double distancetocentre(Point3D& point);
    
    void SetStorageCheck(bool b);
    void SetStoragePoint1(Point3D p);
    void SetStoragePoint2(Point3D p);
    Vector3D InsideNormalVector(Point3D& p, bool debug);
    Vector3D OutsideNormalVector(Point3D& p, bool debug);
    bool IntersectionConcave(Photon& photon);
    Vector3D GetNormalVector(Photon& photon, bool debug);
    
    void SetE(ellipse& ell);
    ellipse& GetE();
    
    bool directioncheck(Vector3D& v, Vector3D& m);
    bool pointonpath(Point3D& point, Photon& photon);
        
    
};


#endif