#include "arc.hpp"

arc::arc(){}

arc::arc(ellipse& elps, double start, double end){
    SetE(elps);
    startangle = start;
    endangle = end;
}

arc::arc(Point3D& centre, double a, double b, double start, double end){
    e.SetCentre(centre);
    e.SetA(a);
    e.SetB(b);
    startangle = start;
    endangle = end;
}

bool
arc::pointonarc(Point3D& point, bool debug){

    if(e.pointcheck(point)){
        //Find angle relative to centre.
        double angle = 0;
        
        Point3D c = this->e.GetCentre();
        
        double a = (point.x - c.x);
        double o = (point.y - c.y);
        
        //cout<<"opposite is "<<o<<" and adjacent is "<<a<<endl;
        
        if(o==0){
            if(a>0) angle = M_PI_2;
            if(a<0) angle = -M_PI_2;
        }
        
        if(a==0){
            if(o>0) angle = 0;
            if(o<0) angle = M_PI;
        }
        
        if(a>0){
            angle = atan(o/a);
        }else{
            if(o<0){
                angle = - (M_PI_2 + atan(o/a));
            }
            else{
                angle = M_PI_2 - atan(o/a);
            }
        }
        
        //cout<<"angle is"<<angle<<endl;
        
        if(angle >=startangle && angle<=endangle)
            
        {
            return true;
        }
        
        
    }
    
    

    
    return false;
}


/*bool
arc::pointonarc(Point3D &point, bool debug){
    if(e.pointcheck(point)){
        if(debug){
            cout<<endl;
            cout<<"Running pointonarc check."<<endl<<endl;
        }
        
        double thetaX = acos((point.x-e.GetCentre().x)/e.GetA()); //Gets into parametric form
        
        if(debug) cout<<"thetaX = "<<thetaX<<endl;
        double thetaY = asin((point.y-e.GetCentre().y)/e.GetB());
        if(debug) cout<<"thetaY = "<<thetaY<<endl;
        if(fabs(fabs(thetaX)-fabs(thetaY))>=1e-3){
            if(debug) cout<<"thetaX and thetaY differ too much"<<endl;
            return false;
        }
        if(thetaY>=0){
            if(thetaX<=endangle){
                return true;
            }
        }
        if(thetaY<=0){
            if(thetaY>=startangle){
                return true;
            }
        }
    }
    
    return false;
}*/

bool
arc::photonarcintersect(Photon &photon, bool debug){
    if(debug){
       cout<<"Checking photonarcintersect!"<<endl;
    }
    
    e.points3D(photon);
    combined e_info = e.GetStorage();
    Point3D p1 = e_info.GetPoint();
    Point3D p2 = e_info.GetPoint2();
    
    bool po1 = 0;
    bool po2 = 0;
    
    if(pointonpath(p1, photon)){
        po1 = 1;
        if(debug){
            cout<<"Po1 value is:"<<po1<<endl;
        }
    }
    
    if(pointonpath(p2, photon)){
        po2 = 1;
        if(debug){
            cout<<"Po2 value is:"<<po2<<endl;
        }
    }
    
    if(debug){
    cout<<"Printpoint - photonarcintersect!"<<endl;
        reader.PrintPoint(p1);
        reader.PrintPoint(p2);
    }
        
    
    if(pointonarc(p1, debug)&&pointonarc(p2, debug)&&po1&&po2){
        if(debug){
            cout<<"Both points are on the arc! Return true!"<<endl;
        }
        
        SetStoragePoint1(p1);
        SetStoragePoint2(p2);
        return true;
    }
    
    if(pointonarc(p1, debug)&&po1){
        if(debug){
            cout<<"Point is on arc! Return true!"<<endl;
        }
        SetStoragePoint1(p1);
        return true;
    }
    
    if(pointonarc(p2, debug)&&po2){
        if(debug){
            cout<<"Point 2 is on arc! Return true!"<<endl;
        }
        SetStoragePoint2(p2);
        return true;
    }
    
    if(debug){
        cout<<"No points on the arc! Return false!"<<endl;
    }
    return false;
}

combined
arc::GetStorage(){
    return storage;
}

Point3D //finds next point of intersection on arc. Perhaps need direction check.
arc::GetNextPoint(Photon& photon, bool debug){
    
    bool check = 0;
    check = photonarcintersect(photon, debug);
    Point3D point1 = GetStorage().GetPoint();
    Point3D point2 = GetStorage().GetPoint2();
    
    
    if(debug){
        cout<<"GetNextPoint Print!"<<endl;
        cout<<"Points generated from photonarcintersect are:"<<endl;
        cout<<"Point1:"<<endl;
        reader.PrintPoint(point1);
        cout<<"Point2:"<<endl;
        reader.PrintPoint(point2);

    }
    if(check){
        
        
        
        double distance1 = photon.GetPosition().distancetopoint(point1);
        double distance2 = photon.GetPosition().distancetopoint(point2);
        
        if(debug){
            cout<<"Distance to point1 is: "<<distance1<<endl;
            cout<<"Distance to point2 is: "<<distance2<<endl;
        }
        
        if(isnan(distance1) && !isnan(distance2)){
            return point2;
        } ;
        if(isnan(distance2) && !isnan(distance1)){
            return point1;
        }
        
        bool bool1 = pointonarc (point1, debug);
        bool bool2 = pointonarc (point2, debug);
        
        Vector3D motion1 = point1 - photon.GetPosition();
        Vector3D motion2 = point2 - photon.GetPosition();
        
        bool bool3 = directioncheck(photon.GetMomentum(), motion1);
        bool bool4 = directioncheck(photon.GetMomentum(), motion2);
        
        if(bool1 && !bool2 && bool3){
            return point1;
        }
        
        if(bool2 && !bool1 && bool4){
            return point2;
        }
        
        if(distance1<=distance2 && bool1 && bool3) return point1;
        if(distance2<distance1 && bool2 && bool4) return point2;
        
    }
    return Point3D(NAN,NAN,NAN);
}

void
arc::setstartangle(double startangle){
    this->startangle = startangle;
}

void
arc::setendangle(double endangle){
    this->endangle = endangle;
}

double&
arc::getstartangle(){
    return startangle;
}

double&
arc::getendangle(){
    return endangle;
}

double
arc::IntersectDistance(Photon &photon, bool debug){
    
    double distance = photon.GetPosition().distancetopoint(GetNextPoint(photon, debug));
    return distance;
}

double
arc::distancetocentre(Point3D &point){
    
    Point2D centre2D(e.GetCentre().x, e.GetCentre().y);
    Point2D intersect2D(point.x, point.y);
    
    return intersect2D.distancetopoint(centre2D);
}

void
arc::SetStorageCheck(bool b){
    storage.SetCheck(b);
}

void
arc::SetStoragePoint1(Point3D p){
    storage.SetPoint(p);
}

void
arc::SetStoragePoint2(Point3D p){
    storage.SetPoint2(p);
}

Vector3D
arc::InsideNormalVector(Point3D &p, bool debug){
    Vector3D normal(NAN,NAN,NAN);
    if(pointonarc(p, debug)){
        normal = e.GetCentre()-p;
        normal.z = 0;
        normal.Normalise();
    }
    return normal;
}

Vector3D
arc::OutsideNormalVector(Point3D &p, bool debug){
    Vector3D normal(NAN,NAN,NAN);
    if(pointonarc(p, debug)){
        normal = p - e.GetCentre();
        normal.z = 0;
        normal.Normalise();
    }
    return normal;
}

//Check if intersection is concave or convec for part. (i.e. if the photon is inside or outside the ellipse.)
bool
arc::IntersectionConcave(Photon& photon){
    bool test = 0;
    
    Point3D& pos = photon.GetPosition();
    
    test = GetE().PointInsideEllipse(pos);
    
    return test;
}

/*bool //Checks if intersection is on concave or convex part of arc. assumes intersection will happen. Returns true for concave.
 arc::IntersectionConcave(Photon &photon){
 double distance = IntersectDistance(photon);
 //cout<<"distance is "<<distance<<endl;
 Point3D position = photon.GetPosition();
 Vector3D momentum = photon.GetMomentum();
 
 Point3D point1 = position + momentum*(0.99999*distance);
 Point3D point2 = position + momentum*(1.00001*distance);
 //reader.PrintPoint(point1);
 //reader.PrintPoint(point2);
 
 Point2D c(e.GetCentre().x, e.GetCentre().y);
 
 Point2D p1(point1.x, point1.y);
 Point2D p2(point2.x, point2.y);
 //reader.Print2DPoint(c);
 //reader.Print2DPoint(p1);
 //reader.Print2DPoint(p2);
 double d1 = distancetocentre(point1);
 double d2 = distancetocentre(point2);
 
 
 if(d1<d2){
 //cout<<"Concave"<<endl;
 return true;
 }
 //cout<<"Convex"<<endl;
 
 return false;
 }*/

Vector3D //If photon intersects arc, and intersection is concave, returns normal vector.
arc::GetNormalVector(Photon &photon, bool debug){
    Point3D nextpoint = GetNextPoint(photon, debug);
    
    if(photonarcintersect(photon, debug) && IntersectionConcave(photon)){
        return InsideNormalVector(nextpoint, debug);
    }
    return OutsideNormalVector(nextpoint, debug);
    
}

void
arc::SetE(ellipse &ell){
    e = ell;
}

ellipse&
arc::GetE(){
    return e;
}

bool
arc::directioncheck(Vector3D &v, Vector3D &m){ //checks that two vectors are in the same direction
    bool test = 0;
    double dot = Dot(v.Normalise(),m.Normalise());
    
    if(fabs(1-dot)<1e-6){
        test = 1;
    }
    
    return test;
}

bool //checks that a point is on the path of a photon
arc::pointonpath(Point3D &point, Photon& photon){
    
    Point3D& position = photon.GetPosition();
    Vector3D momentum = photon.GetMomentum();
    
    Vector3D path = point-position;
    
    bool check = directioncheck(path, momentum);
    
    return check;
}