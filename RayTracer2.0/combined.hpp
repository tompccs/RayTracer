#ifndef combined_hpp
#define combined_hpp

#include <stdio.h>
#include "Point3D.h"

class combined{
    bool check;
    Point3D point;
    Point3D point2;
    
    
public:
    
    combined();
    
    //Stores a bool, and a point together
    void SetCheck(bool set);
    bool GetCheck();
    void SetPoint(Point3D set);
    Point3D GetPoint();
    void SetPoint2(Point3D set);
    Point3D GetPoint2();
    
};

#endif