#include "arc.hpp"

arc::arc(){}

arc::arc(ellipse& elps, double start, double end){
    e = elps;
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
    
    e.points3D(photon);
    combined e_info = e.GetStorage();
    Point3D p1 = e_info.GetPoint();
    Point3D p2 = e_info.GetPoint2();
    
    //cout<<"Printpoint!!!!"<<endl;
    //reader.PrintPoint(p1);
    //reader.PrintPoint(p2);

    
    if(pointonarc(p1)&&pointonarc(p2)){
        //cout<<"Both points are on the arc! Return true!"<<endl;
        SetStoragePoint1(p1);
        SetStoragePoint2(p2);
        return true;
    }
    
    if(pointonarc(p1)){
        //cout<<"Point is on arc! Return true!"<<endl;
        SetStoragePoint1(p1);
        return true;
    }

    if(pointonarc(p2)){
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
    bool check = photonarcintersect(photon);
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
        
        if(bool1 && !bool2){
            return point1;
        }
        
        if(bool2 && !bool1){
            return point2;
        }
        
        if(distance1<=distance2 && bool1) return point1;
        if(distance2<distance1 && bool2) return point2;
        
    }
    return Point3D(NAN,NAN,NAN);
}

double
arc::IntersectDistance(Photon &photon){
    return photon.GetPosition().distancetopoint(GetNextPoint(photon));
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
        normal.Normalise();
    }
    return normal;
}

Vector3D
arc::OutsideNormalVector(Point3D &p){
    Vector3D normal(NAN,NAN,NAN);
    if(pointonarc(p)){
        normal = p - e.GetCentre();
        normal.Normalise();
    }
    return normal;
}

bool //Checks if intersection is on concave or convex part of arc. assumes intersection will happen. Returns true for concave.
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
}

Vector3D //If photon intersects arc, and intersection is concave, returns normal vector.
arc::GetNormalVector(Photon &photon){
    Point3D nextpoint = GetNextPoint(photon);
    
    if(photonarcintersect(photon) && IntersectionConcave(photon)){
        return InsideNormalVector(nextpoint);
    }
    return OutsideNormalVector(nextpoint);
    
}