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
tube::FindIntersections2(Photon &photon, bool debug, combined& values){
    //debug = 0;
    int intersect2D = base.FindIntersections(photon, debug, values);
    
    if(debug){
        cout<<endl<<"Photon->Tube Intersection (3D)"<<endl<<endl;
        cout<<"Projection of intersections from 2D to 3D."<<endl;
    }
    
    if(intersect2D == 0){
        if(debug)cout<<"No 2D intersection. Return 0 Intersections."<<endl;
        return 0;
    }
    
    if(intersect2D == 1){
        double distance2D = values.GetDistance1();
        if(debug)cout<<"Single 2D distance is"<<distance2D<<endl;
        double distance3D = Project2D3D(distance2D, photon);
        values.SetDistance1(distance3D);
        if(debug)cout<<"Projection into 3D distance is "<<distance3D<<endl;
        
        return 1;
    }
    
    if(intersect2D == 2){
        double distance2D1 = values.GetDistance1();
        if(debug)cout<<" 2D distance 1 is "<<distance2D1<<endl;
        double distance3D1 = Project2D3D(distance2D1, photon);
        values.SetDistance1(distance3D1);
        if(debug)cout<<"Projection into 3D distance is "<<distance3D1<<endl;
        

        double distance2D2 = values.GetDistance2();
        if(debug)cout<<" 2D distance 2 is "<<distance2D2<<endl;
        double distance3D2 = Project2D3D(distance2D2, photon);
        values.SetDistance2(distance3D2);
        if(debug)cout<<"Projection into 3D distance is "<<distance3D2<<endl;
        
        return 2;
    }
    
    return 0;
}

int
tube::FindIntersections(Photon &photon, bool debug, combined &values){
    int result = 0;
    
    //Get variable values
    Vector3D mom = photon.GetMomentum();
    Point3D pos = photon.GetPosition();
    double r = base.GetRadius();
    Point2D P0 = base.GetCentre();
    
    double eps = 1e-4;
    
    double dx = mom.x;
    double dy = mom.y;
    //double dz = mom.z;
    
    double x0 = pos.x;
    double y0 = pos.y;
    //double z0 = pos.z;
    
    double rx = P0.x;
    double ry = P0.y;
    
    double X = x0 - rx;
    double Y = y0 - ry;
    
    double t1, t2;
    
    double A = (dx*dx + dy*dy);
    double B = 2*(dx*X + dy*Y);
    double C = X*X + Y*Y - r*r;
    
    double DET = B*B - 4*A*C;
    
    t1 = (-B + sqrt(DET))/(2*A);
    t2 = (-B - sqrt(DET))/(2*A);
    
    bool pos1 = 0;
    bool pos2 = 0;
    
    if(t1>eps){
        pos1 = 1;
    }
    
    if(t2>eps){
        pos2 = 1;
    }
    
    
    if(DET<0){
        values.SetDistance1(INFINITY);
        values.SetDistance2(INFINITY);
        result = 0;
    }else if(DET==0 && pos1){
        values.SetDistance1(t1);
        values.SetDistance2(INFINITY);
        result = 1;
    }else if(DET>0){
        
        if(pos1&&pos2){
            if(t1>t2){
                values.SetDistance1(t1);
                values.SetDistance2(t2);
            }else{
                values.SetDistance1(t2);
                values.SetDistance2(t1);
            }
            result = 2;
        }
        
        if(pos1&&!pos2){
            values.SetDistance1(t1);
            values.SetDistance2(INFINITY);
            result = 1;
        }
        if(pos2&&!pos1){
            values.SetDistance1(t2);
            values.SetDistance2(INFINITY);
            result = 1;
        }
    }
    
    return result;
}


double
tube::Project2D3D(double &distance2D, Photon &photon){
    
    Vector3D mom = photon.GetMomentum().Normalise();
    Vector3D mom2D = Vector3D(mom.x,mom.y,0);
    mom2D.Normalise();
    
    double distance3D;
    
    distance3D = distance2D / Dot(mom,mom2D);
    
    return distance3D;
}

