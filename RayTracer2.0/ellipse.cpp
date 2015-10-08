#include "ellipse.hpp"

ellipse::ellipse(){} //empty ellipse

ellipse::ellipse(Point3D centre, double a, double b){ //define an ellipse by its centre point and axial lengths
    this->centre = centre;
    this->a = a;
    this->b = b;
}

bool
ellipse::pointcheck(Point3D& point){ //check if a point falls on an ellipse
    double x = point.x;
    double y = point.y;
    
    // double z = point.z;
    
    double eps = 1e-6;
    
    double h = this->centre.x;
    double k = this->centre.y;
    
    
    if (((x-h)*(x-h)/(a*a))+(y-k)*(y-k)/(b*b)<=(1+eps)&&((x-h)*(x-h)/(a*a))+(y-k)*(y-k)/(b*b)>=(1-eps)) {
        return true;
    } else {
        return false;
    }
    
}

bool
ellipse::ellipselineintersectcheck(Photon &photon){ //checks if ellipse and line intersect
    
    //first rearrange position/momentum of photon (in 2D) to y=mx + c form
    
    double m;
    double c;
    
    
    //check for infinite m values (Line x=C)
    
    double dybydx = (photon.GetMomentum().y/photon.GetMomentum().x);
    
    if(dybydx<=1e5){ //If line x = C. Check intersection simply
        m = dybydx;
    }else{
        double x = photon.GetPosition().x;
        if(fabs(x)<=a){
            return true;
        }else{
            return false;
        }
    }
    
    c = photon.GetPosition().y-(photon.GetPosition().x*m);
    
    //now apply line-intersection point formula to get points of intersection (2D)
    
    double h = this->centre.x;
    double k = this->centre.y;
    
    //case 1: h = 0; k = 0; c = 0.
    if(h==0&&k==0&&c==0){
        double denom = sqrt(a*a*m*m + b*b);
        double x1_num = a*b;
        double x2_num = 0 - a*b;
        double y1_num = a*b*m;
        double y2_num = 0- a*b*m;
        
        double x1 = x1_num/denom;
        double x2 = x2_num/denom;
        double y1 = y1_num/denom;
        double y2 = y2_num/denom;
        
        Point3D point1(x1,y1,0);
        Point3D point2(x2,y2,0);
        
        if (pointcheck(point1)){
            return true;
        } else if(pointcheck(point2)){
            return true;
        } else{
            return false;
        }
        
    }
    
    //case 2: h=0; k=0; c!=0.
    
    if(h==0&&k==0&&c!=0){
        double denom = a*a*m*m + b*b;
        double x1_num = 0 - a*a*m*c + a*b*sqrt(a*a*m*m + b*b - c*c);
        double x2_num = 0 - a*a*m*c - a*b*sqrt(a*a*m*m + b*b - c*c);
        double y1_num = b*b*c + a*b*m*sqrt(a*a*m*m + b*b - c*c);
        double y2_num = b*b*c - a*b*m*sqrt(a*a*m*m + b*b - c*c);
        
        double x1 = x1_num/denom;
        double x2 = x2_num/denom;
        double y1 = y1_num/denom;
        double y2 = y2_num/denom;
        
        
        Point3D point1(x1,y1,0);
        Point3D point2(x2,y2,0);

        if (pointcheck(point1)){
            return true;
        } else if(pointcheck(point2)){
            return true;
        } else{
            return false;
        }

    }
    
    //case 3: h!= 0; k!= 0; c!= 0;
    
    double eps = c-k;
    double delta = c + (m*h);
    
    double x1, x2, y1, y2;
    
    double x1_num = h*b*b - eps*a*a*m + a*b*sqrt(a*a*m*m + b*b - delta*delta - k*k + 2*delta*k);
    double x2_num = h*b*b - eps*a*a*m*m - a*b*m*sqrt(a*a*m*m + b*b - delta*delta - k*k + 2*delta*k);
    double x_den = a*a*m*m + b*b;
    
    x1 = x1_num/x_den;
    x2 = x2_num/x_den;
    
    double y1_num = b*b*delta + k*a*a*m*m + a*b*m*sqrt(a*a*m*m+b*b-delta*delta-k*k+2*delta*k);
    double y2_num = b*b*delta + k*a*a*m*m - a*b*m*sqrt(a*a*m*m+b*b-delta*delta-k*k+2*delta*k);
    double y_den = a*a*m*m + b*b;
    
    y1 = y1_num/y_den;
    y2 = y2_num/y_den;
    
    Point3D point1(x1,y1,0);
    Point3D point2(x2,y2,0);
    
    Test* print = new Test;

    print->PrintPoint(point1);
    print->PrintPoint(point2);
    
    if(pointcheck(point1)){
        return true;
    };
    if(pointcheck(point2)){
        return true;
    };
    
    return false;
}

Point3D
ellipse::firstintersectpoint(Photon& photon){
    
    return Point3D(0,0,0);
};