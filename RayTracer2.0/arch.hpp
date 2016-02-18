#ifndef arch_hpp
#define arch_hpp

#include <stdio.h>
#include "tube.hpp"

class arch{
    tube Tube;
    double startangle;
    double endangle;
    
public:
    void SetTube(tube& t);
    void SetStart(double& s);
};


#endif