#ifndef circle_hpp
#define circle_hpp

#include <stdio.h>
#include "Point2D.h"
#include "Vector2D.h"
#include "Photon.h"
#include "Test.h"
#include "combined.hpp"

class circle{
    
    Point2D centre;
    double radius;
    Test reader;
    
        
    
public:
    
    circle();
    
    circle(Point2D& c, double& r);
    
    void SetCentre(Point2D& c);
    Point2D& GetCentre();
    
    void SetRadius(double& r);
    double& GetRadius();
    
    int FindIntersections(Photon& photon, bool debug, combined& values);
    int FindIntersections2(Photon& photon, bool debug, combined& values);

    
    
};

#endif