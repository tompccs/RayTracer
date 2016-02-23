#ifndef curved_lsc_hpp
#define curved_lsc_hpp

#include <stdio.h>
#include "arch.hpp"
#include "Sheet.h"

class curvedlsc{
    
    arch inside;
    arch outside;
    Sheet top;
    Sheet bottom;
    Sheet start;
    Sheet end;
    
    double refractiveindex;
    bool Photoninside;
    
public:
    curvedlsc();
    void Set(Point3D centre, double radius, double length, double thickness, double height, double refractiveindex);
    
};

#endif