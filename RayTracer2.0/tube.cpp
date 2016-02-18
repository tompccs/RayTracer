#include "tube.hpp"

circle&
tube::GetCircle(){
    return base;
}

double&
tube::GetHeight(){
    return height;
}

void
tube::SetCircle(circle &c){
    base = c;
}

void
tube::SetHeight(double &h){
    height = h;
}

int
tube::FindIntersections(Photon &photon, bool debug, combined& values){
    
    int intersect2D = base.FindIntersections(photon, debug, values);
    
    if(intersect2D == 0){
        if(debug)cout<<"No 2D intersection. Return 0 Intersections."<<endl;
        return 0;
    }
    
    if(intersect2D == 1){
        double distance2D = values.GetDistance1();
        if(debug)cout<<"Single 2D distance is"<<distance2D<<endl;
        double distance3D = Project2D3D(distance2D, photon);
        values.SetDistance1(distance3D);
        if(debug)cout<<"Projection into 3D distance is"<<distance3D<<endl;
        
        return 1;
    }
    
    if(intersect2D == 2){
        double distance2D1 = values.GetDistance1();
        if(debug)cout<<" 2D distance 1 is"<<distance2D1<<endl;
        double distance3D1 = Project2D3D(distance2D1, photon);
        values.SetDistance1(distance3D1);
        if(debug)cout<<"Projection into 3D distance is"<<distance3D1<<endl;
        

        double distance2D2 = values.GetDistance2();
        if(debug)cout<<" 2D distance 2 is"<<distance2D2<<endl;
        double distance3D2 = Project2D3D(distance2D2, photon);
        values.SetDistance2(distance3D2);
        if(debug)cout<<"Projection into 3D distance is"<<distance3D2<<endl;
        
        return 2;
    }
    
    return 0;
}

double
tube::Project2D3D(double &distance2D, Photon &photon){
    
    Vector3D mom = photon.GetMomentum();
    Vector3D mom2D = Vector3D(mom.x,mom.y,0);
    mom2D.Normalise();
    
    double distance3D;
    
    distance3D = distance2D * Dot(mom,mom2D);
    
    return distance3D;
}

