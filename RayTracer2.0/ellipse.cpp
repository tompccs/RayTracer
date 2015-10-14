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
    
    double eps = 1e-6; //needs calculation for how large this actually needs to be
    
    //eps is the margin of error. this allows for rounding errors in the calculation
    //tests needed for how large this margin of error should be. can be calculated (see numerical methods notes)
    
    double h = this->centre.x;
    double k = this->centre.y;
    
    double lhs = ((x-h)*(x-h)/(a*a))+((y-k)*(y-k)/(b*b)); //calculates lhs of (x-h)^2/a^2 + (y-k)^2/b^2 = 1 (Ellipse equation.)
    
    if (fabs(1-lhs)<eps){ //calculates difference between 1 and LHS of ellipse
        return true;
    }
    return false;
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

bool
ellipse::LineOnEllipseIntersection(Photon &photon){
    bool result = false;
    Point2D intersection (0,0);
    
    Vector3D P0 = photon.GetPosition();
    Vector3D P1 = photon.GetPosition()+photon.GetMomentum();
    
    //used for quadratic
    
    double aa = 0, bb = 0, cc = 0, m = 0;
    
    double a = this->a;
    double b = this->b;
    
    //case where m is not equal to infinity
    if (P0.x != P1.x){
        Vector3D slope = photon.GetMomentum();
        double m = slope.y/slope.x;
        double c = P0.y - m*P0.x;
        
        aa = b*b + a*a*m*m;
        bb = 2*a*a*c*m - a*a*centre.y*m-2*centre.x*b*b;
        cc = b*b*centre.x*centre.x + a*a*c*c - 2*a*a*centre.y*c + a*a*centre.y*centre.y - a*a*b*b;
    }
    
    //case where m = infinity
    
    else{
        aa = a*a;
        bb = -2*centre.y*a*a;
        cc = -a*a*b*b + b*b*(P0.x - centre.x)*(P0.x-centre.x);
    }
    
    //Calculates the determinant d
    double d = bb*bb - 4*aa*cc;
    
    //If d > 0. we have intersections.
    
    Point2D intersection1;
    Point2D intersection2;
    
    if (d > 0.0){
        if (P0.x != P1.x){
            double x1 = (-bb + sqrt(d)) / (2*aa);
            double y1 = P0.y + m*(x1 - P0.x);
            intersection1 = Point2D(x1, y1);
            
            double x2 = (-bb - sqrt(d)) / (2 * aa);
            double y2 = P0.y + m * (x2 - P0.x);
            intersection2 = Point2D(x2, y2);
        }
        
        else{
            double y1 = (-bb + sqrt(d)) / (2 * aa);
            intersection1 = Point2D(P0.x,y1);
            double y2 = (-bb - sqrt(d)) / (2 * aa);
            intersection2 = Point2D(P0.x,y2);
        }
        
        //Determine closest point to P0
        double distance1 = (intersection1.x - P0.x)*(intersection1.x - P0.x) + (intersection1.y - P0.y)*(intersection1.y - P0.y);
        double distance2 = (intersection2.x - P0.x)*(intersection2.x - P0.x) + (intersection2.y - P0.y)*(intersection2.y - P0.y);
        
        Point2D closestPoint(0,0);
        
        if (distance1 <= distance2){
            closestPoint = intersection1;
        }
        else{
            closestPoint = intersection2;
        }
        
        //Determine if the closest point is on the line.
        if (photon.PointInline2D(closestPoint)){
            result = true;
            intersection = closestPoint;
        }
    }
    
    return result;
    
}

combined //this method checks for intersection, and saves points of intersection.
ellipse::photonellipseintersect(Photon& photon){
    
    //Set default values.
    combined checkpoint;
    checkpoint.SetCheck(0);
    checkpoint.SetPoint(Point3D(0,0,0));
    checkpoint.SetPoint2(Point3D(0,0,0));
    
    double h = this->centre.x;
    double k = this->centre.y;

    
    //first rearrange position/momentum of photon (in 2D) to y=mx + c form
    
    double m;
    double c;
    
    //case 1: check for x=C (vertical line in 2D)
    
    
    double dybydx = (photon.GetMomentum().y/photon.GetMomentum().x);
    
    if(dybydx<=1e5){ //If line is not x = C. Checks intersection simply
        m = dybydx;
    } else {
        double x = photon.GetPosition().x;
        if(fabs(x)<=a){
            checkpoint.SetCheck(1);
            double y1 = k + sqrt(k*k + b*b* sqrt((1-(x-h)*(x-h)/a*a)));
            double y2 = k - sqrt(k*k + b*b* sqrt((1-(x-h)*(x-h)/a*a)));
            checkpoint.SetPoint(Point3D(x,y1,0));
            checkpoint.SetPoint2(Point3D(x,y2,0));
            return checkpoint;
        }else{
            return checkpoint;
        }
    }
    
    c = photon.GetPosition().y-(photon.GetPosition().x*m);

    //case 2: h = 0; k = 0; c = 0.
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
            checkpoint.SetCheck(1);
            checkpoint.SetPoint(point1);
            if(pointcheck(point2)){
                checkpoint.SetCheck(1);
                checkpoint.SetPoint2(point2);
            }
        } else{
            return checkpoint;
        }
    }
    
    //case 3: h=0; k=0; c!=0.
    
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
            checkpoint.SetCheck(1);
            checkpoint.SetPoint(point1);
            if(pointcheck(point2)){
                checkpoint.SetCheck(1);
                checkpoint.SetPoint2(point2);
            }
            return checkpoint;
        }
        else{
            return checkpoint;
        }
    }
    
    //case 4: h!= 0; k!= 0; c!= 0;
    
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
    
    
    if(pointcheck(point1)){
        checkpoint.SetCheck(1);
        checkpoint.SetPoint(point1);
        if(pointcheck(point2)){
            checkpoint.SetCheck(1);
            checkpoint.SetPoint2(point2);
        }
        return checkpoint;
    }
    
    return checkpoint;
}

Point3D
ellipse::nextpoint(Photon &photon, combined &checker){
    Point3D current = photon.GetPosition();
    
    double distance = current.distancetopoint(checker.GetPoint());
    double distance2 = current.distancetopoint(checker.GetPoint2());
    
    if(distance <= distance2 ){
        return checker.GetPoint();
    }
    
    if(distance>=distance2){
        return checker.GetPoint2();
    }
    
    return current;
}

Point3D
ellipse::nextpoint3D(Photon& photon){
    combined ellipsefind = photonellipseintersect(photon); //Finds points on 2D space where ellipse is crossed
    Point3D A = nextpoint(photon, ellipsefind); //Finds which point comes first
    Point3D X2D = Point3D(photon.GetPosition().x,photon.GetPosition().y,0); //position of photon in 2d space
    Vector3D XA = A-X2D; //vector from position to point of intersection in 2d space
    double magXA = Magnitude(XA);
    double XAdotXB = Dot(XA,photon.GetMomentum());
    double distancetoB = magXA*magXA/XAdotXB;
    return (photon.GetPosition()+(photon.GetMomentum()*distancetoB));
}

void
ellipse::SetCentre(Point3D &centre){
    this->centre = centre;
}

void
ellipse::SetA(double A){
    a = A;
}

void
ellipse::SetB(double B){
    b = B;
}

Point3D
ellipse::GetCentre(){
    return centre;
}

double
ellipse::GetA(){
    return a;
}

double
ellipse::GetB(){
    return b;
}