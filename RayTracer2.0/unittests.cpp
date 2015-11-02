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

void
unittest::arctest(){
    Test reader;
    
    Point3D centre(13,5,0);
    double a = 3;
    double b = 2;
    
    ellipse e(centre, a, b);
    
    Point3D position(15.9,1,0);
    Point3D momentum(0,1,0);
    
    Photon photon(momentum,position);
    
    arc testarc(e,- M_PI_4, M_PI_4);
    
    e.points3D(photon);
    
    //Point3D point1 = e.GetStorage().GetPoint();
    //Point3D point2 = e.GetStorage().GetPoint2();
    
    //reader.PrintBool(testarc.photonarcintersect(photon));
    
    //reader.PrintPoint(point1);
    //reader.PrintPoint(point2);
    //reader.PrintBool(testarc.pointonarc(point1));
    //reader.PrintBool(testarc.pointonarc(point2));
    
    //cout<<"Print first point of intersection: "<<endl;
    //reader.PrintPoint(testarc.GetNextPoint(photon));
    
    //testarc.IntersectionConcave(photon);
    
    reader.PrintVector(testarc.GetNormalVector(photon));

    
}