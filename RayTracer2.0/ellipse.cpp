#include "ellipse.hpp"

ellipse::ellipse(){} //empty ellipse

ellipse::ellipse(Point3D centre, double a, double b){ //define an ellipse by its centre point and axial lengths
    this->centre = centre;
    this->a = a;
    this->b = b;
}

bool
ellipse::pointcheck(Point3D point){ //check if a point falls on an ellipse
    
    bool debug = 0;
    
    double x = point.x;
    double y = point.y;
    
    if(debug){
        cout<<"Performing point check on: "<<x<<" "<<y<<endl;
    }
    
    // double z = point.z;
    
    double eps =  1e-4; //Epsilon acts as an error threshold. Worth playing around with.
    
    double h = this->centre.x;
    double k = this->centre.y;
    
    double lhs = ((x-h)*(x-h)/(a*a))+((y-k)*(y-k)/(b*b)); //calculates lhs of (x-h)^2/a^2 + (y-k)^2/b^2 = 1 (Ellipse equation.)
    
    if(debug){
        cout<<"x2 + y2 ="<<lhs<<endl;
    }
    
    if (fabs(1-lhs)<eps){ //calculates difference between 1 and LHS of ellipse
        if(debug){cout<<"Point is on ellipse!"<<endl;}
        return true;
    }
    return false;
}

/*bool
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
 }*/

bool
ellipse::LineOnEllipseIntersection(Photon &photon){
    bool debug = 0;
    
    Test reader;
    
    bool result = false;
    Point2D intersection (0,0);
    Point2D closest(0,0);
    Point2D farthest(0,0);
    
    
    Point3D P0 = photon.GetPosition();
    Point3D P1 = photon.GetPosition()+photon.GetMomentum();
    
    if(debug){
        cout<<"Position is: "<<endl;
        reader.PrintPoint(P0);
        cout<<"Position + Momentum is: "<<endl;
        reader.PrintPoint(P1);
    }
    
    
    //used for quadratic
    
    double aa = 0, bb = 0, cc = 0, M = 0;
    
    double a = this->a;
    if(debug) cout<<"This A ="<<a<<endl;
    double b = this->b;
    if(debug) cout<<"This B ="<<b<<endl;
    
    //case where m is not equal to infinity
    
    if (fabs(P0.x-P1.x)>1e-3){
        if(debug) cout<<"not vertical line"<<endl;
        Vector3D slope = photon.GetMomentum();
        if(debug) cout<<"Momentum vector is:"<<endl;
        if(debug) reader.PrintVector(photon.GetMomentum());
        M = slope.y/slope.x;
        if(debug) cout<<"M (gradient) is equal to: "<<M<<endl;
        double c = P0.y - M*P0.x;
        if(debug) cout<<"c = "<<c<<endl;
        
        aa = b*b + a*a*M*M;
        bb = (2*a*a*c*M) - (2*a*a*centre.y*M) - (2*centre.x*b*b);
        cc = b*b*centre.x*centre.x + a*a*c*c - 2*a*a*centre.y*c + a*a*centre.y*centre.y - a*a*b*b;
    }
    
    //case where M = infinity
    
    else{
        if(debug) cout<<"Vertical line"<<endl;
        aa = a*a;
        bb = -2*centre.y*a*a;
        cc = -a*a*b*b + b*b*(P0.x - centre.x)*(P0.x-centre.x);
    }
    
    if(debug) cout<<"aa ="<<aa<<" and bb ="<<bb<<" and cc ="<<cc<<endl;
    
    //Calculates the determinant d
    double d = bb*bb - 4*aa*cc;
    
    //If d > 0. we have intersections.
    
    Point2D intersection1;
    Point2D intersection2;
    
    if(debug) cout<<"d has value: "<<d<<endl;
    
    if(debug) cout<<"P0.x:"<<P0.x<<" and P1.x:"<<P1.x<<endl;
    
    if (d > 0.0){
        if (fabs(P0.x-P1.x)>0){
            if(debug) cout<<"P0.x != P1.x"<<endl;
            double x1 = (-bb + sqrt(d)) / (2*aa);
            double y1 = P0.y + M*(x1 - P0.x);
            if(debug) cout<<"P0.y ="<<P0.y<<" and m="<<M<<" x1:"<<x1<<" and P0.x= "<<P0.x<<endl;
            intersection1.x = x1;
            if(debug) cout<<"x1 ="<<x1<<endl;
            intersection1.y = y1;
            if(debug) cout<<"y1 ="<<y1<<endl;
            if(debug)reader.Print2DPoint(intersection1);
            
            double x2 = (-bb - sqrt(d)) / (2 * aa);
            double y2 = P0.y + M*(x2 - P0.x);
            intersection2.x = x2;
            intersection2.y = y2;
        }
        
        else{
            double k = centre.y;
            double h = centre.x;
            
            double y1 = k + b* sqrt(1 - ((P0.x-h)*(P0.x-h)/(a*a)));
            double y2 = k - b* sqrt(1 - ((P0.x-h)*(P0.x-h)/(a*a)));
            
            if(debug)cout<<" k= "<<k<<" b= "<<b<<" P0.x ="<<P0.x<<" h= "<<h<<" a= "<<a<<endl;
            if(debug) cout<<"y1 = "<<y1;
            if(debug) cout<<"y2 = "<<y2;
            
            intersection1.x = P0.x;
            intersection1.y = y1;
            intersection2.x = P0.x;
            intersection2.y = y2;
            
            y1 = (-bb + sqrt(d)) / (2 * aa);
            //cout<<"y1 = "<<y1<<endl;
            intersection1.x = P0.x;
            intersection1.y = y1;
            y2 = (-bb - sqrt(d)) / (2 * aa);
            //cout<<"y2 = "<<y2<<endl;
            intersection2.x = P0.x;
            intersection2.y = y2;
        }
        
        //Determine closest point to P0
        double distance1 = (intersection1.x - P0.x)*(intersection1.x - P0.x) + (intersection1.y - P0.y)*(intersection1.y - P0.y);
        double distance2 = (intersection2.x - P0.x)*(intersection2.x - P0.x) + (intersection2.y - P0.y)*(intersection2.y - P0.y);
        
        Point2D closestPoint(0,0);
        Point2D nextPoint(0,0);
        
        if (distance1 <= distance2){
            closest = intersection1;
            farthest = intersection2;
        }
        else{
            closest = intersection2;
            farthest = intersection1;
        }
        
        //Determine if the closest point is on the line.
        if (photon.PointInline2D(closestPoint)){
            result = true;
            intersection = closestPoint;
            
        }else if (photon.PointInline2D(nextPoint)){
            result = true;
            intersection = nextPoint;
        }
    }
    //cout<<"Intersection point 1 is: ";
    //reader.Print2DPoint(intersection1);
    //cout<<"Intersection point 2 is: ";
    //reader.Print2DPoint(intersection2);
    //cout<<"Test for line-ellipse intersection returns: ";
    //reader.PrintBool(result);
    
    
    //cout<<"Test that points 1 and 2 are on the curve."<<endl;
    
    Point3D check1(intersection1.x, intersection1.y, 0);
    Point3D check2(intersection2.x, intersection2.y, 0);
    
    //reader.PrintBool(pointcheck(check1));
    //reader.PrintBool(pointcheck(check2));
    
    storage.SetCheck(result);
    storage.SetPoint(check1);
    storage.SetPoint2(check2);
    
    return result;
    
}

/*bool
ellipse::LineOnEllipseIntersection(Photon& photon){
    //if(debug){cout<<"Testing for line circle intersection."<<endl;}
    
    bool result = 0;
    int solutions = 0;
    
    
    double cx, cy, radius;
    cx = centre.x;
    cy = centre.y;
    radius = this->a;
    
    Point2D intersectionpoint1, intersectionpoint2;
    double dx, dy, A, B, C, det, t;
    
    dx = photon.GetMomentum().x;
    dy = photon.GetMomentum().y;
    
    A = dx*dx + dy*dy;
    B = 2*(dx*(photon.GetPosition().x - cx) + dy * (photon.GetPosition().y - cy));
    C = (photon.GetPosition().x - cx) * (photon.GetPosition().x - cx) + (photon.GetPosition().y - cy)*(photon.GetPosition().y - cy) - radius * radius;
    
    det = B * B - 4 * A * C;
    
    if(A <= 1e-7 || det<0){
        //No real solutions
        intersectionpoint1 = Point2D(NAN,NAN);
        intersectionpoint2 = Point2D(NAN,NAN);
        storage.SetPoint(Point3D(intersectionpoint1.x,intersectionpoint1.y, 0));
        storage.SetPoint2(Point3D(intersectionpoint2.x,intersectionpoint2.y,0));
        result = 0;
        
    }else if(det == 0){
        //One solution
        t = -B / (2*A);
        intersectionpoint1 = Point2D(photon.GetPosition().x + photon.GetMomentum().x*t,photon.GetPosition().y + photon.GetMomentum().y*t);
        intersectionpoint2 = Point2D(NAN,NAN);
        
        storage.SetPoint(Point3D(intersectionpoint1.x,intersectionpoint1.y, 0));
        storage.SetPoint2(Point3D(intersectionpoint2.x,intersectionpoint2.y,0));
        result = 1;
        solutions = 1;
        
    }else{
        // Two solutions.
        
        double t1 = -B + sqrt(det) / (2*A);
        intersectionpoint1 = Point2D(photon.GetPosition().x + photon.GetMomentum().x*t1,photon.GetPosition().y + photon.GetMomentum().y*t1);
        double t2 = -B - sqrt(det) / (2*A);
        intersectionpoint2 = Point2D(photon.GetPosition().x + photon.GetMomentum().x*t2,photon.GetPosition().y + photon.GetMomentum().y*t2);
        
        if(t1<=t2){
            storage.SetPoint(Point3D(intersectionpoint1.x,intersectionpoint1.y, 0));
            storage.SetPoint2(Point3D(intersectionpoint2.x,intersectionpoint2.y,0));
        }
        
        else{
             storage.SetPoint2(Point3D(intersectionpoint1.x,intersectionpoint1.y, 0));
             storage.SetPoint(Point3D(intersectionpoint2.x,intersectionpoint2.y,0));
        }
        
        result = 1;
        solutions = 2;
    }
    
    storage.SetCheck(result);
    
    return result;
}*/



bool
ellipse::PointInsideEllipse(Point3D &point){
    double x = point.x;
    double y = point.y;
    double cx = centre.x;
    double cy = centre.y;
    
    bool test = 0;
    
    double ell = ((x-cx)*(x-cx)/a*a) + ((y-cy)*(y-cy)/b*b);
    if(fabs(ell-1)<1e-3){
        test = 1;
    }
    
    return test;
}

bool
ellipse::DirectionCheck(Photon &photon, Point3D& point){
    
    Point3D P0 = photon.GetPosition();
    Point3D P1 = photon.GetPosition()+photon.GetMomentum();
    
    
    if(fabs(P0.distancetopoint(P1) + P1.distancetopoint(point) - P0.distancetopoint(point)) < 1e-3){
        return true;
    }
    
    return false;
}

combined
ellipse::GetStorage(){
    return storage;
}

int //Projects vector into 3D space.
ellipse::points3D(Photon& photon){
    
    LineOnEllipseIntersection(photon); //Finds points of intersection (if any) in 2D space). Assume done.
    
    Point3D A = storage.GetPoint(); //Gets point A from Storage.
    Point3D X2D = Point3D(photon.GetPosition().x,photon.GetPosition().y,0); //Position of photon in 2D space
    Vector3D XA = A-X2D; //vector from position to point of intersection in 2d space
    
    double magXA = Magnitude(XA);
    double XAdotXB = Dot(XA,photon.GetMomentum());
    double distancetoB = magXA*magXA/XAdotXB;
    storage.SetPoint((photon.GetPosition()+(photon.GetMomentum()*distancetoB)));
    
    Point3D A2 = storage.GetPoint2();
    Vector3D XA2 = A2-X2D; //vector from position to point of intersection in 2d space
    double magXA2 = Magnitude(XA2);
    double XA2dotXB = Dot(XA2,photon.GetMomentum());
    double distancetoB2 = magXA2*magXA2/XA2dotXB;
    storage.SetPoint2((photon.GetPosition()+(photon.GetMomentum()*distancetoB2)));
    
    return 0;
}

void
ellipse::SetCentre(Point3D &centre){
    this->centre = centre;
}

void
ellipse::SetA(double A){
    this->a = A;
}

void
ellipse::SetB(double B){
    this->b = B;
}

Point3D&
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