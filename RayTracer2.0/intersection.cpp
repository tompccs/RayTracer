#include "Intersection.hpp"

double&
Intersection::GetDistance(){
    return Distance;
}

Vector3D&
Intersection::GetNormal(){
    return Normal;
}

Point3D&
Intersection::GetPoint(){
    return Point;
}

combined&
Intersection::GetValues(){
    return values;
}

void
Intersection::ArchIntersect(Photon &p, arch &a, bool debug){
    
    double d1 = values.GetDistance1();
    double d2 = values.GetDistance2();
    double d;
    d = d;
    
    if(d1<=d2){
        d = d1;
    }else{
        d = d2;
    }
    
    Distance = d;
    
    Point = p.GetPosition() + p.GetMomentum() * d;
    
    Point2D cen = a.GetTube().GetCircle().GetCentre();
    Point3D cen3(cen.x, cen.y, Point.z);
    
    Normal = (cen3  - Point).Normalise();
    
    if(debug){
        cout<<endl<<"Full arch intersection. Received distances are:"<<endl;
        cout<<d1<<" and "<<d2<<endl;
        cout<<"Comparison gives shortest distance, and hence next intersection distance of "<<d<<endl;
        cout<<"This gives an intersection point of "<<endl;
        reader.PrintPoint(Point);
        cout<<"At this point, the normal vector is:"<<endl;
        reader.PrintVector(Normal);
    }
}

void
Intersection::SheetIntersect(Photon &p, Sheet &s, bool debug){
    
    
    if(debug){
        cout<<"Distance to sheet intersect is:"<<endl;
    }
}
