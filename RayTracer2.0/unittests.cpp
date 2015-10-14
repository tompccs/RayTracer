#include "unittests.hpp"

//Method tests for a point, to see if it lies on an ellipse.
//Check negative values

void
unittest::ellipse_pointcheck(){
    
    Point3D centre(0,0,0);
    double a = 3;
    double b = 4;
    
    ellipse e(centre,a,b);
    
    Point3D testpoint(0,0,0);
    
    if(e.pointcheck(testpoint)){
        cout<<"Point lies on ellipse."<<endl;
    }else{
        cout<<"Point does not lie on ellipse."<<endl;
    }
    
}