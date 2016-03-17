#ifndef tube_hpp
#define tube_hpp

#include <stdio.h>
#include "circle.hpp"

class tube{
    circle base;
    double height;
    Test reader;
    
public:
    circle& GetCircle();
    double& GetHeight();
    void SetCircle(circle& c);
    void SetHeight(double& h);
    int FindIntersections(Photon& photon, bool debug, combined& values);
    int FindIntersections2(Photon& photon, bool debug, combined& values);
    double Project2D3D(double& distance2D, Photon& photon);
    
};

#endif