#ifndef curvedbox_hpp
#define curvedbox_hpp

#include <stdio.h>
#include "arc.hpp"
#include "Sheet.h"
#include "Photon.h"

class curvedbox{
    arc insidearc;
    arc outsidearc;
    Sheet startsheet;
    Sheet endsheet;
    Sheet topplane;
    Sheet bottomplane;
    double refractiveindex;
    
public:
    
    curvedbox();
    curvedbox(arc& central, double height, double width, double refractiveindex);
    
    
    
    
};

#endif