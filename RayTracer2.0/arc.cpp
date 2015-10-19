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
        cout<<"Point check confirmed."<<endl;
        double thetaX = acos((point.x-e.GetCentre().x)/e.GetA()); //Gets into parametric form
        cout<<"thetaX = "<<thetaX<<endl;
        double thetaY = asin((point.y-e.GetCentre().y)/e.GetB());
        cout<<"thetaY = "<<thetaY<<endl;
        if(fabs(fabs(thetaX)-fabs(thetaY))>=1e-7){
            cout<<"thetaX and thetaY differ too much"<<endl;
            return false;
        }
        cout<<"thetaY = "<<thetaY<<endl;
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

/*bool
arc::photonarcintersect(Photon &photon){
    combined findpoints = e.photonellipseintersect(photon);
    Point3D p1 = findpoints.GetPoint();
    Point3D p2 = findpoints.GetPoint2();
    
    if(pointonarc(p1)&&pointonarc(p2)){
        cout<<"Both points are on the arc! Return true!"<<endl;
    }
    
    if(pointonarc(p1)){
        cout<<"Point is on arc! Return true!"<<endl;
        return true;
    }

    if(pointonarc(p2)){
        cout<<"Point 2 is on arc! Return true!"<<endl;
        return true;
    }

    cout<<"No points on the arc! Return false!"<<endl;
    return false;
}*/