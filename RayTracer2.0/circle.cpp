#include "circle.hpp"

circle::circle(){}

circle::circle(Point2D& c, double& r){
    centre = c;
    radius = r;
}

void
circle::SetCentre(Point2D &c){
    centre = c;
}

Point2D&
circle::GetCentre(){
    return centre;
}

void
circle::SetRadius(double &r){
    radius = r;
}

double&
circle::GetRadius(){
    return radius;
}

int
circle::FindIntersections(Photon &photon, bool debug, combined& values){
    
    //Method calculates whether a photon will intersect circle in 2D space.
    //Returns number of intersections.
    
    double cx, cy, rad;
    
    cx = centre.x;
    cy = centre.y;
    
    Point2D p1,p2;
    double distance1, distance2;
    
    
    double dx,dy,A,B,C,det,t;
    
    dx = photon.GetMomentum().x;
    dy = photon.GetMomentum().y;
    
    p1 = Point2D(photon.GetPosition().x, photon.GetPosition().y);
    p1 = Point2D(photon.GetPosition().x + dx, photon.GetPosition().y + dy);
    
    if(debug){
        cout<<"Photon->Circle Intersection (2D)"<<endl;
        cout<<"Checking for intersections between Photon with"<<endl;
        cout<<"Position: ";
        reader.PrintPoint(photon.GetPosition());
        cout<<" and Momentum: ";
        reader.PrintVector(photon.GetMomentum());
        cout<<"and circle with radius "<<radius<<" and centre";
        reader.Print2DPoint(centre);
    }

    
    A = dx*dx + dy*dy;
    B = 2 * (dx * (p1.x - cx) + dy * (p1.y - cy));
    C = (p1.x - cx) * (p1.x - cx) + (p1.y - cy) * (p1.y - cy) - rad*rad;
    
    det = B * B - 4 * A * C;
    
    if(debug){
        cout<<"Calculated determinant is "<<det<<endl;
    }
    
    if((A <= 1e-7) || (det < 0)){
        //No real solutions
        cout<<"No real solutions."<<endl;
        return 0;
    }
    
    else if (det == 0){
        t= -B / (2*A);
        distance1 = t;
        if(debug){ cout<<"1 real solution at distance "<<t<<endl;}
        values.SetDistance1(t);
        return 1;
    }
    
    else{
        t = ((-B + sqrt(det))/ 2*A);
        distance1 = t;
        values.SetDistance1(distance1);
        t = ((-B - sqrt(det))/ 2*A);
        distance2 = t;
        values.SetDistance2(distance2);
        cout<<"2 real solutions at distance "<<distance1<<" and "<<distance2<<endl;
        return 2;
    }
    
    return 0;
}