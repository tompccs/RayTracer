#include "arch.hpp"

void
arch::SetTube(tube t){
    Tube = t;
}

void
arch::SetStart(double& s){
    startangle = s;
}

void
arch::SetEnd(double& e){
    endangle = e;
}

tube&
arch::GetTube(){
    return Tube;
}

double&
arch::GetStart(){
    return startangle;
}

double&
arch::GetEnd(){
    return endangle;
}

int
arch::FindIntersection(Photon& p, bool debug, combined values){
    int intersects = Tube.FindIntersections(p, debug, values);
    
    if(intersects == 0){
        values.SetDistance1(INFINITY);
        values.SetDistance2(INFINITY);
    }
    
    if(intersects == 1){
        double distance;
        
        Point3D point = p.GetPosition()+p.GetMomentum()*distance;
        
        double angle1 = CheckAngle(point);
        
        if(!((angle1<=endangle) && (angle1>=startangle))){
            distance = INFINITY;
        }
        
        values.SetDistance1(distance);
        values.SetDistance2(INFINITY);
        //Run check on intersection.
    }
    
    if(intersects == 2){
        double distance1;
        double distance2;
        
        Point3D point = p.GetPosition()+p.GetMomentum()*distance1;
        Point3D point2 = p.GetPosition()+p.GetMomentum()*distance2;
        
        double angle1 = CheckAngle(point);
        
        if(!((angle1<=endangle) && (angle1>=startangle))){
            distance1 = INFINITY;
        }
        
        double angle2 = CheckAngle(point2);
        
        if(!((angle2<=endangle) && (angle2>=startangle))){
            distance2 = INFINITY;
        }

        
        values.SetDistance1(distance1);
        values.SetDistance2(distance2);
        //Run check on intersections
    }
    
    return 0;
}

double //Returns the angle relative to centre.
arch::CheckAngle(Point3D &p){
    
    double angle = NAN;
    
    double x = p.x - Tube.GetCircle().GetCentre().x;
    double y = p.y - Tube.GetCircle().GetCentre().y;
    
    bool x_sign = signbit(x);
    bool y_sign = signbit(y);
    
    if(x_sign && y_sign){
        angle = atan(y/x);
    }
    
    if((!x_sign) && y_sign){
        angle = M_PI_2 + atan(-x/y);
    }
    
    if((!x_sign) && (!y_sign)){
        angle = (-M_PI_2) - atan(x/y);
    }
    
    if((x_sign) && (!y_sign)){
        angle = atan(y/x);
    }
    
    return angle;
}