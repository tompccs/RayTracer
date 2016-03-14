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
arch::FindIntersection(Photon& p, bool debug, combined& values){
    int intersects = Tube.FindIntersections(p, debug, values);
     //debug = 0;
    if(debug){
        
        cout<<endl;
        cout<<"Photon->Arch Intersection (3D)"<<endl<<endl;
        cout<<"Checking angles lie between "<<startangle<<" and "<<endangle<<endl;
    }
    
    if(intersects == 0){
        if(debug){
            cout<<"No received intersections. "<<endl;
            
            
        }
        values.SetDistance1(INFINITY);
        values.SetDistance2(INFINITY);
    }
    
    if(intersects == 1){
        double distance = values.GetDistance1();
        
        
        Point3D point = p.GetPosition()+p.GetMomentum()*distance;
        
        if(debug){
            cout<<"One intersection passed at point"<<endl;
            reader.PrintPoint(point);
        }
        
        
        double angle1 = CheckAngle(point, debug);
        
        if(debug){
            cout<<"Angle output is "<<angle1<<endl;
        }
        
        if(!((angle1<=endangle) && (angle1>=startangle))){
            distance = INFINITY;
        }
        
        values.SetDistance1(distance);
        values.SetDistance2(INFINITY);
        //Run check on intersection.
    }
    
    if(intersects == 2){
        double distance1 = values.GetDistance1();
        double distance2 = values.GetDistance2();
        
        
        Point3D point = p.GetPosition()+p.GetMomentum()*distance1;
        Point3D point2 = p.GetPosition()+p.GetMomentum()*distance2;
        
        if(debug){
            cout<<"Two intersections passed at point"<<endl;
            reader.PrintPoint(point);
            cout<<" and point "<<endl;
            reader.PrintPoint(point2);
        }
        
        
        double angle1 = CheckAngle(point, debug);
        double angle2 = CheckAngle(point2, debug);
        
        
        if(debug){
            cout<<"Angle from point 1 is:"<<angle1<<endl;
            cout<<"Angle from point 2 is:"<<angle2<<endl;
        }
        
        bool b1 = 1;
        bool b2 = 1;
        
        if(!((angle1<=endangle) && (angle1>=startangle))){
            distance1 = INFINITY;
            b1 = 0;
        }
        
        
        
        
        if(!((angle2<=endangle) && (angle2>=startangle))){
            distance2 = INFINITY;
            b2 = 0;
        }
        
        
        
        values.SetDistance1(distance1);
        values.SetDistance2(distance2);
        
        if(debug){
            cout<<"Distance 1 is: "<<distance1<<" and Distance 2 is: "<<distance2<<endl;
        }
        
        if(b1^b2){
            return 1;
        }
        
        if(b1&&b2){
            return 2;
        }
        
        //Run check on intersections
    }
    
    return 0;
}

double //Returns the angle relative to centre.
arch::CheckAngle(Point3D &p, bool debug){
    
    
    double angle = NAN;
    
    double x = p.x - Tube.GetCircle().GetCentre().x;
    double y = p.y - Tube.GetCircle().GetCentre().y;
    
    bool x_sign = !signbit(x);
    bool y_sign = !signbit(y);
    
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