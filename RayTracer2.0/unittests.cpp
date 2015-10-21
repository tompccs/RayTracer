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

void
unittest::LineOnEllipseIntersection(){
    
    Point3D centre(13,5,0);
    double a = 3;
    double b = 2;
    
    ellipse e(centre, a, b);
    
    Point3D position(1,3.005,0);
    Vector3D momentum(1,0,0);
    
    Photon testphoton;
    testphoton.SetPosition(position);
    testphoton.SetMomentum(momentum);
    
    e.LineOnEllipseIntersection(testphoton);
    
    
}

void
unittest::Ellipse_Points3D(){
    
    Point3D centre(13,5,0);
    double a = 3;
    double b = 2;
    
    ellipse e(centre, a, b);
    
    Point3D position(2,2,0);
    Vector3D momentum(11,3,-1);
    
    Photon testphoton;
    testphoton.SetPosition(position);
    testphoton.SetMomentum(momentum);
    
    e.points3D(testphoton);
    
    Test reader;
    
    reader.PrintPoint(e.GetStorage().GetPoint());
    reader.PrintPoint(e.GetStorage().GetPoint2());
    
}