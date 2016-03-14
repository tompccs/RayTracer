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
circle::FindIntersections2(Photon &photon, bool debug, combined& values){
    //debug = 0;
    //Method calculates whether a photon will intersect circle in 2D space.
    //Returns number of intersections.
    
    double eps = 0;
    
    double cx, cy, rad;
    
    Vector2D mom2D(photon.GetMomentum().x,photon.GetMomentum().y);
    mom2D.Normalize();
    
    rad = radius;
    
    cx = centre.x;
    cy = centre.y;
    
    Point2D p1,p2;
    double distance1, distance2;
    
    
    double dx,dy,A,B,C,det,t;
    
    dx = mom2D.x;
    dy = mom2D.y;
    
    p1 = Point2D(photon.GetPosition().x, photon.GetPosition().y);
    p2 = Point2D(photon.GetPosition().x + dx, photon.GetPosition().y + dy);
    
    if(debug){
        cout<<"Photon->Circle Intersection (2D)"<<endl<<endl;
        cout<<"Checking for intersections between Photon"<<endl;
        cout<<"Position: ";
        reader.PrintPoint(photon.GetPosition());
        cout<<"and Momentum: ";
        reader.PrintVector(photon.GetMomentum());
        cout<<"with circle with radius "<<radius<<" and centre ";
        reader.Print2DPoint(centre);
    }
    
    
    A = dx*dx + dy*dy;
    B = 2 * (dx * (p1.x - cx) + dy * (p1.y - cy));
    C = (p1.x - cx) * (p1.x - cx) + (p1.y - cy) * (p1.y - cy) - rad*rad;
    
    det = B * B - 4 * A * C;
    
    if(debug){
        cout<<"Calculated determinant is "<<det<<" as A = "<<A<<" B = "<<B<<" C = "<<C<<endl;
    }
    
    if((A <= 1e-5) || (det < 0)){
        //No real solutions
        if(debug){cout<<"No real solutions."<<endl;}
        return 0;
    }
    
    else if (det == 0){
        //1 solution
        t= -B / (2*A);
        distance1 = t;
        if(debug){ cout<<"1 real solution at distance "<<t<<endl;}
        if(t>eps){
            values.SetDistance1(t);
            return 1;
        }else{
            values.SetDistance1(INFINITY);
            return 0;
        }
    }
    
    else{
        bool b1 = 1, b2 = 1;
        
        t = ((-B + sqrt(det))/ 2*A);
        
        if(t>eps){
            distance1 = t;
            values.SetDistance1(distance1);
        }
        else{
            b1 = 0;
            distance1 = INFINITY;
        }
        
        t = ((-B - sqrt(det))/ 2*A);
        
        if(t>eps){
            distance2 = t;
            values.SetDistance2(distance2);
        }else{
            distance2 = INFINITY;
            b2 = 0;
        }
        
        
        
        if(b1&&b2){
            if(debug){
                cout<<"2 real solutions at distances "<<values.GetDistance1()<<" and "<<values.GetDistance2()<<endl;
            }
            return 2;
        }
        
        if(b1^b2){
            if(b1){
                if(debug){
                    cout<<"1 real solution at distance "<<values.GetDistance1()<<endl;
                }
            }
            
            if(b2){
                if(debug){
                    cout<<"1 real solution at distance"<<values.GetDistance2()<<endl;
                }
            }
            return 1;
        }
    }
    
    if(debug){
        cout<<"No real solutions for 2D intersection in positive t direction."<<endl;
    }
    
    return 0;
}

int
circle::FindIntersections(Photon &photon, bool debug, combined &values){
    int result = 0;
    double eps = 1e-4;
    
    Vector2D D(photon.GetMomentum().x, photon.GetMomentum().y);
    D.Normalize();
    
    if(debug){reader.Print2DVector(D);}
    
    
    Vector2D P(photon.GetPosition().x, photon.GetPosition().y);
    
    Point2D C(centre.x,centre.y);
    double R = radius;
    
    Vector2D DEL = P - C;
    
    double DDotDel = Dot(D,DEL);
    double MagD = Magnitude(D);
    //double MagDEL = Magnitude(DEL);
    double DDotD = Dot(D,D);
    double DELDotDEL = Dot(DEL,DEL);
    
    
    double delta = (DDotDel*DDotDel) - DDotD*(DELDotDEL-R*R);
    
    if(debug){
        cout<<"Photon->Circle Intersection (2D)"<<endl<<endl;
        cout<<"Checking for intersections between Photon"<<endl;
        cout<<"Position: ";
        reader.PrintPoint(photon.GetPosition());
        cout<<"and Momentum: ";
        reader.PrintVector(photon.GetMomentum());
        cout<<"with circle with radius "<<radius<<" and centre ";
        reader.Print2DPoint(centre);
    }

    
    if(delta<0){
        //No real solutions
        return 0;
    }
    
    else if(delta==0){
        //1 solution
        
        double t = (-DDotDel)/MagD*MagD;
        
        if(debug){ cout<<"1 real solution at distance "<<t<<endl;}
        if(t>eps){
            values.SetDistance1(t);
            return 1;
        }else{
            values.SetDistance1(INFINITY);
            return 0;
        }
    }
    
    else if(delta>0){
        double t = (-DDotDel + sqrt(delta))/(MagD*MagD);
        double t2 = (-DDotDel - sqrt(delta))/(MagD*MagD);
        double distance1, distance2;
        bool b1 = 1, b2 = 1;
        
        if(t>eps){
            values.SetDistance1(t);
        }else{
            b1 = 0;
            distance1 = INFINITY;
            values.SetDistance1(distance1);
        }
        
        if(t2>eps){
            values.SetDistance2(t2);
        }else{
            distance2 = INFINITY;
            b2 = 0;
            values.SetDistance2(t2);

        }
        
        if(b1&&b2){
            if(debug){
                cout<<"2 real solutions at distances "<<values.GetDistance1()<<" and "<<values.GetDistance2()<<endl;
            }
            return 2;
        }
        
        if(b1^b2){
            if(b1){
                if(debug){
                    cout<<"1 real solution at distance "<<values.GetDistance1()<<endl;
                }
            }
            
            if(b2){
                if(debug){
                    cout<<"1 real solution at distance"<<values.GetDistance2()<<endl;
                }
            }
            return 1;
        }
    }
    
    return result;
}


