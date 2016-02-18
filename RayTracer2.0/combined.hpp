#ifndef combined_hpp
#define combined_hpp

#include <stdio.h>
#include "Point3D.h"

class combined{
    bool check;
    bool check2;
    Point3D point;
    Point3D point2;
    double distance1;
    double distance2;
    
    
    
    
public:
    
    combined();
    
    //Stores a bool, and a point together
    void SetCheck(bool set);
    bool GetCheck();
    void SetCheck2(bool set);
    bool GetCheck2();
    void SetPoint(Point3D set);
    Point3D GetPoint();
    void SetPoint2(Point3D set);
    Point3D GetPoint2();
    void SetDistance1(double d);
    double& GetDistance1();
    void SetDistance2(double d);
    double& GetDistance2();
    
};

#endif