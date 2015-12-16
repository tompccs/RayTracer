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

arc::arc(double cx, double cy, double cz, double a, double b, double st, double en){
    
}


bool
arc::pointonarc(Point3D &point){
    if(e.pointcheck(point)){
        //cout<<"Point check confirmed."<<endl;
        double thetaX = acos((point.x-e.GetCentre().x)/e.GetA()); //Gets into parametric form
        //cout<<"thetaX = "<<thetaX<<endl;
        double thetaY = asin((point.y-e.GetCentre().y)/e.GetB());
        //cout<<"thetaY = "<<thetaY<<endl;
        if(fabs(fabs(thetaX)-fabs(thetaY))>=1e-4){
            //cout<<"thetaX and thetaY differ too much"<<endl;
            return false;
        }
        //cout<<"thetaY = "<<thetaY<<endl;
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
}

bool
arc::photonarcintersect(Photon &photon){
    
    //cout<<"Checking!"<<endl;
    
    e.points3D(photon);
    combined e_info = e.GetStorage();
    Point3D p1 = e_info.GetPoint();
    Point3D p2 = e_info.GetPoint2();
    
    bool po1 = 0;
    bool po2 = 0;
    
    if(pointonpath(p1, photon)){
        po1 = 1;
    }
    
    if(pointonpath(p2, photon)){
        po2 = 1;
    }
    
    //cout<<"Printpoint!!!!"<<endl;
    //reader.PrintPoint(p1);
    //reader.PrintPoint(p2);
    
    
    if(pointonarc(p1)&&pointonarc(p2)&&po1&&po2){
        //cout<<"Both points are on the arc! Return true!"<<endl;
        SetStoragePoint1(p1);
        SetStoragePoint2(p2);
        return true;
    }
    
    if(pointonarc(p1)&&po1){
        //cout<<"Point is on arc! Return true!"<<endl;
        SetStoragePoint1(p1);
        return true;
    }
    
    if(pointonarc(p2)&&po2){
        //cout<<"Point 2 is on arc! Return true!"<<endl;
        SetStoragePoint2(p2);
        return true;
    }
    
    //cout<<"No points on the arc! Return false!"<<endl;
    return false;
}

combined
arc::GetStorage(){
    return storage;
}

Point3D //finds next point of intersection on arc. Perhaps need direction check.
arc::GetNextPoint(Photon& photon){
    bool check = 0;
    check = photonarcintersect(photon);
    Point3D point1 = GetStorage().GetPoint();
    Point3D point2 = GetStorage().GetPoint2();
    
    
    //cout<<"GetNextPoint Print!"<<endl;
    //reader.PrintPoint(point1);
    //reader.PrintPoint(point2);
    
    if(check){
        
        double distance1 = photon.GetPosition().distancetopoint(point1);
        double distance2 = photon.GetPosition().distancetopoint(point2);
        
        if(isnan(distance1) && !isnan(distance2)){
            return point2;
        } ;
        if(isnan(distance2) && !isnan(distance1)){
            return point1;
        }
        
        bool bool1 = pointonarc (point1);
        bool bool2 = pointonarc (point2);
        
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
arc::IntersectDistance(Photon &photon){
    
    double distance = photon.GetPosition().distancetopoint(GetNextPoint(photon));
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
arc::InsideNormalVector(Point3D &p){
    Vector3D normal(NAN,NAN,NAN);
    if(pointonarc(p)){
        normal = e.GetCentre()-p;
        normal.z = 0;
        normal.Normalise();
    }
    return normal;
}

Vector3D
arc::OutsideNormalVector(Point3D &p){
    Vector3D normal(NAN,NAN,NAN);
    if(pointonarc(p)){
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
arc::GetNormalVector(Photon &photon){
    Point3D nextpoint = GetNextPoint(photon);
    
    if(photonarcintersect(photon) && IntersectionConcave(photon)){
        return InsideNormalVector(nextpoint);
    }
    return OutsideNormalVector(nextpoint);
    
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