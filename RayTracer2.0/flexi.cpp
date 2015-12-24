#include "flexi.hpp"

flexi::flexi(){};

void
flexi::Set(Point3D &C, double &T, double &L, double &H, double &R, double &N){
    centre = C;
    t = T;
    l = L;
    h = H;
    r = R;
    n = N;
}

void
flexi::SetCentre(Point3D& point){
    centre = point;
}

Point3D&
flexi::GetCentre(){
    return centre;
}

void
flexi::SetThickness(double &number){
    t = number;
}

double&
flexi::GetThickness(){
    return t;
}

void
flexi::SetLength(double &number){
    l = number;
}

double&
flexi::GetLength(){
    return l;
}

void
flexi::SetHeight(double &number){
    h = number;
}

void
flexi::SetRadius(double &number){
    r = number;
}

double&
flexi::GetRadius(){
    return r;
}

void
flexi::SetRefractiveIndex(double &number){
    n = number;
}

double&
flexi::GetRefractiveIndex(){
    return n;
}

void
flexi::SetPhotonInside(bool &b){
    PhotonInside = b;
}

bool&
flexi::GetPhotonInside(){
    return PhotonInside;
}

void
flexi::Build(){
    
    double inside_radius = r - (t/2);
    double outside_radius = r + (t/2);
    
    //Definition of inside curve
    
    inside.SetCentre(this->centre);
    inside.SetA(inside_radius);
    inside.SetB(inside_radius);
    
    //Definition of outside curve
    
    outside.SetCentre(this->centre);
    outside.SetA(outside_radius);
    outside.SetB(outside_radius);
    
    double x0 = centre.x;
    double y0 = centre.y;
    double z0 = centre.z;
    
    //Definition of bottom sheet
    
    Point3D bottomA(x0-outside_radius, y0-outside_radius, z0);
    Point3D bottomB(x0+outside_radius, y0-outside_radius, z0);
    Point3D bottomC(x0-outside_radius, y0+outside_radius, z0);
    
    //Definition of top sheet
    
    Point3D topA(x0-outside_radius, y0+outside_radius, z0 + h);
    Point3D topB(x0+outside_radius, y0+outside_radius, z0 + h);
    Point3D topC(x0-outside_radius, y0-outside_radius, z0 + h);
    
    bottom.SetA(bottomA);
    bottom.SetB(bottomB);
    bottom.SetC(bottomC);
    
    top.SetA(topA);
    top.SetB(topB);
    top.SetC(topC);
    
    //Definition of start and end sheets
    
    double theta = l/r;
    double theta_2 = theta/2;
    
    startangle = theta_2;
    endangle = -theta_2;
    
    Point3D startA(x0 + inside_radius*sin(startangle), y0 + inside_radius*cos(startangle), z0);
    Point3D startB(x0 + outside_radius*sin(startangle), y0 + outside_radius*cos(startangle), z0);
    Point3D startC(x0 + inside_radius*sin(startangle), y0 + inside_radius*cos(startangle), z0+h);
    
    Point3D endA(x0 + outside_radius*sin(endangle), y0 + outside_radius*cos(endangle), z0);
    Point3D endB(x0 + inside_radius*sin(endangle), y0 + inside_radius*cos(endangle), z0);
    Point3D endC(x0 + outside_radius*sin(endangle), y0 + outside_radius*cos(endangle), z0+h);
    
    start.SetA(startA);
    start.SetB(startB);
    start.SetC(startC);
    
    end.SetA(endA);
    end.SetB(endB);
    end.SetC(endC);
    
    
}

double
flexi::DistancetoCurvedIntersect(Photon &photon, ellipse &ell){
    double result = INFINITY;
    Point3D point1;
    Point3D point2;
    
    //Find determinant for intersection.
    double det = CurvedDet(photon, ell);
    
    //If det is less than or equal to 0, no intersection.
    if(det<=0){
        return INFINITY;
    }
    else{
        point1 = FlatPointOne(photon, ell, det);
        point2 = FlatPointTwo(photon, ell, det);
    }
    
    //Project this onto 3D path of photon
    
    point1 = Project3D(photon, point1);
    point2 = Project3D(photon, point2);
    
    bool check1 = 0;
    bool check1_1 = 0; //Check point lies on ellipse
    bool check1_2 = 0; //Check point lies on 3D ellipse
    bool check1_3 = 0; //Check point lies on arc
    bool check1_4 = 0; //Check point lies on path of photon
    
    bool check2 = 0;
    bool check2_1 = 0; //Same checks but for point two
    bool check2_2 = 0;
    bool check2_3 = 0;
    bool check2_4 = 0;
    
    
    //Checks point lies on ellipse in 2D
    
    if(ell.pointcheck(point1)){
        check1_1 = 1;
    }
    
    if(ell.pointcheck(point2)){
        check2_1 = 1;
    }
    
    //Checks point lies between z0 and z0 + h in z axis
    
    if(point1.z>= ell.GetCentre().z && point1.z<=ell.GetCentre().z + h){
        check1_2 = 1;
    }
    
    if(point2.z>= ell.GetCentre().z && point2.z<=ell.GetCentre().z + h){
        check2_2 = 1;
    }
    
    //Find angle for point
    
    double angle1 = FindAngle(point1, ell);
    double angle2 = FindAngle(point2, ell);
    
    //Check angle lies between start and end angle
    
    if(startangle>= angle1 && angle1 <= endangle){
        check1_3 = 1;
    }
    
    if(startangle>= angle2 && angle2 <= endangle){
        check2_3 = 1;
    }
    
    //Check point lies on path of photon
    
    check1_4 = pointonpath(point1, photon);
    check2_4 = pointonpath(point2, photon);
    
    if(check1_1 && check1_2 && check1_3 && check1_4){
        check1 = 1;
    }
    
    if(check2_1 && check2_2 && check2_3 && check2_4){
        check2 = 1;
    }
    
    if(check1 && !check2){
        result = point1.distancetopoint(photon.GetPosition());
    }
    
    if(check2 && !check1){
        result = point2.distancetopoint(photon.GetPosition());
    }
    
    else if(check1 && check2){
        double distance1 = point1.distancetopoint(photon.GetPosition());
        double distance2 = point2.distancetopoint(photon.GetPosition());
        
        if(distance1 <= distance2) result = distance1;
        if(distance2 < distance1) result = distance2;
    }
    
    return result;
}

double
flexi::CurvedDet(Photon &photon, ellipse &ell){
    double det = 0;
    
    Point2D intersection (0,0);
    
    Point3D P0 = photon.GetPosition();
    Point3D P1 = photon.GetPosition()+photon.GetMomentum();
    
    //used for quadratic
    
    double aa = 0, bb = 0, cc = 0, M = 0;
    
    double a = ell.GetA();
    double b = ell.GetB();
    
    //case where m is not equal to infinity
    
    if (fabs(P0.x-P1.x)>=1e-4){
        //cout<<"not vertical line"<<endl;
        Vector3D slope = photon.GetMomentum();
        M = slope.y/slope.x;
        //cout<<"M (gradient) is equal to: "<<M<<endl;
        double c = P0.y - M*P0.x;
        //cout<<"c = "<<c<<endl;
        
        aa = b*b + a*a*M*M;
        bb = 2*a*a*c*M - 2*a*a*centre.y*M-2*centre.x*b*b;
        cc = b*b*centre.x*centre.x + a*a*c*c - 2*a*a*centre.y*c + a*a*centre.y*centre.y - a*a*b*b;
    }
    
    //case where M = infinity
    
    else{
        //cout<<"Vertical line"<<endl;
        aa = a*a;
        bb = -2*centre.y*a*a;
        cc = -a*a*b*b + b*b*(P0.x - centre.x)*(P0.x-centre.x);
    }
    
    //cout<<"aa ="<<aa<<" and bb ="<<bb<<" and cc ="<<cc<<endl;
    
    //Calculates the determinant d
    det = bb*bb - 4*aa*cc;
    
    return det;
}

Point3D
flexi::FlatPointOne(Photon& photon, ellipse& ell, double& det){
    
    Point2D intersection (0,0);
    
    Point3D P0 = photon.GetPosition();
    Point3D P1 = photon.GetPosition()+photon.GetMomentum();
    
    
    //used for quadratic
    
    double aa = 0, bb = 0, cc = 0, M = 0;
    
    double a = ell.GetA();
    //cout<<"This A ="<<a<<endl;
    double b = ell.GetB();
    //cout<<"This B ="<<b<<endl;
    
    //case where m is not equal to infinity
    
    if (fabs(P0.x-P1.x)>=1e-4){
        //cout<<"not vertical line"<<endl;
        Vector3D slope = photon.GetMomentum();
        M = slope.y/slope.x;
        //cout<<"M (gradient) is equal to: "<<M<<endl;
        double c = P0.y - M*P0.x;
        //cout<<"c = "<<c<<endl;
        
        aa = b*b + a*a*M*M;
        bb = 2*a*a*c*M - 2*a*a*centre.y*M-2*centre.x*b*b;
        cc = b*b*centre.x*centre.x + a*a*c*c - 2*a*a*centre.y*c + a*a*centre.y*centre.y - a*a*b*b;
    }
    
    //case where M = infinity
    
    else{
        //cout<<"Vertical line"<<endl;
        aa = a*a;
        bb = -2*centre.y*a*a;
        cc = -a*a*b*b + b*b*(P0.x - centre.x)*(P0.x-centre.x);
    }
    
    //cout<<"aa ="<<aa<<" and bb ="<<bb<<" and cc ="<<cc<<endl;
    
    //Calculates the determinant d
    double d = bb*bb - 4*aa*cc;
    
    //If d > 0. we have intersections.
    
    Point2D intersection1;
    Point2D intersection2;
    
    //cout<<"d has value: "<<d<<endl;
    
    //cout<<"P0.x:"<<P0.x<<" and P1.x:"<<P1.x<<endl;
    
    if (d > 0.0){
        if (fabs(P0.x-P1.x)>1e-4){
            //cout<<"P0.x != P1.x"<<endl;
            double x1 = (-bb + sqrt(d)) / (2*aa);
            double y1 = P0.y + M*(x1 - P0.x);
            //cout<<"P0.y ="<<P0.y<<" and m="<<M<<" x1:"<<x1<<" and P0.x= "<<P0.x<<endl;
            intersection1.x = x1;
            //cout<<"x1 ="<<x1<<endl;
            intersection1.y = y1;
            //cout<<"y1 ="<<y1<<endl;
            //reader.Print2DPoint(intersection1);
            
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
            
            intersection1.x = P0.x;
            intersection1.y = y1;
            intersection2.x = P0.x;
            intersection2.y = y2;
            
        }
        
    }
    
    return Point3D(intersection1.x,intersection1.y,0);
}

Point3D
flexi::FlatPointTwo(Photon& photon, ellipse& ell, double& det){
    
    Point2D intersection (0,0);
    
    Point3D P0 = photon.GetPosition();
    Point3D P1 = photon.GetPosition()+photon.GetMomentum();
    
    
    //used for quadratic
    
    double aa = 0, bb = 0, cc = 0, M = 0;
    
    double a = ell.GetA();
    //cout<<"This A ="<<a<<endl;
    double b = ell.GetB();
    //cout<<"This B ="<<b<<endl;
    
    //case where m is not equal to infinity
    
    if (fabs(P0.x-P1.x)>=1e-4){
        //cout<<"not vertical line"<<endl;
        Vector3D slope = photon.GetMomentum();
        M = slope.y/slope.x;
        //cout<<"M (gradient) is equal to: "<<M<<endl;
        double c = P0.y - M*P0.x;
        //cout<<"c = "<<c<<endl;
        
        aa = b*b + a*a*M*M;
        bb = 2*a*a*c*M - 2*a*a*centre.y*M-2*centre.x*b*b;
        cc = b*b*centre.x*centre.x + a*a*c*c - 2*a*a*centre.y*c + a*a*centre.y*centre.y - a*a*b*b;
    }
    
    //case where M = infinity
    
    else{
        //cout<<"Vertical line"<<endl;
        aa = a*a;
        bb = -2*centre.y*a*a;
        cc = -a*a*b*b + b*b*(P0.x - centre.x)*(P0.x-centre.x);
    }
    
    //cout<<"aa ="<<aa<<" and bb ="<<bb<<" and cc ="<<cc<<endl;
    
    //Calculates the determinant d
    double d = bb*bb - 4*aa*cc;
    
    //If d > 0. we have intersections.
    
    Point2D intersection1;
    Point2D intersection2;
    
    //cout<<"d has value: "<<d<<endl;
    
    //cout<<"P0.x:"<<P0.x<<" and P1.x:"<<P1.x<<endl;
    
    if (d > 0.0){
        if (fabs(P0.x-P1.x)>1e-4){
            //cout<<"P0.x != P1.x"<<endl;
            double x1 = (-bb + sqrt(d)) / (2*aa);
            double y1 = P0.y + M*(x1 - P0.x);
            //cout<<"P0.y ="<<P0.y<<" and m="<<M<<" x1:"<<x1<<" and P0.x= "<<P0.x<<endl;
            intersection1.x = x1;
            //cout<<"x1 ="<<x1<<endl;
            intersection1.y = y1;
            //cout<<"y1 ="<<y1<<endl;
            //reader.Print2DPoint(intersection1);
            
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
            
            intersection1.x = P0.x;
            intersection1.y = y1;
            intersection2.x = P0.x;
            intersection2.y = y2;
            
        }
        
    }
    
    return Point3D(intersection2.x,intersection2.y,0);
}

Point3D
flexi::Project3D(Photon& photon, Point3D& point){
    
    
    Point3D A = point;
    Point3D X2D = Point3D(photon.GetPosition().x,photon.GetPosition().y,0); //position of photon in 2d space
    Vector3D XA = A-X2D; //vector from position to point of intersection in 2d space
    
    double magXA = Magnitude(XA);
    double XAdotXB = Dot(XA,photon.GetMomentum());
    double distancetoB = magXA*magXA/XAdotXB;
    
    return (photon.GetPosition()+(photon.GetMomentum()*distancetoB));
}

double
flexi::FindAngle(Point3D &point, ellipse &ell){
    double angle = 0;
    
    Point3D c = ell.GetCentre();
    
    double o = point.x - c.x;
    double a = point.y - c.y;
    
    if(a==0){
        if(o>0) return M_PI_2;
        if(o<0) return -M_PI_2;
    }
    
    if(o==0){
        if(a>0) return 0;
        if(a<0) return M_PI;
    }
    
    if(o>0){
        angle = atan(o/a);
    }else{
        if(a<0){
            angle = - (M_PI_2 + atan(o/a));
        }
        else{
            angle = M_PI_2 - atan(o/a);
        }
    }
    
    return angle;
}

bool //checks that a point is on the path of a photon
flexi::pointonpath(Point3D &point, Photon& photon){
    
    Point3D& position = photon.GetPosition();
    Vector3D momentum = photon.GetMomentum();
    
    Vector3D path = point-position;
    
    bool check = directioncheck(path, momentum);
    
    return check;
}

bool
flexi::directioncheck(Vector3D &v, Vector3D &m){ //checks that two vectors are in the same direction
    bool test = 0;
    double dot = Dot(v.Normalise(),m.Normalise());
    
    if(fabs(1-dot)<1e-4){
        test = 1;
    }
    
    return test;
}

void
flexi::CalculateDistances(Photon& photon){
    insidedistance = DistancetoCurvedIntersect(photon, inside);
    
    outsidedistance = DistancetoCurvedIntersect(photon, outside);
    
    if(PhotonInside && top.GetIntersectionTest(&photon)){
        topdistance = top.IntersectionDistance(&photon);
    }else{
        topdistance = INFINITY;
    }

    if(PhotonInside && bottom.GetIntersectionTest(&photon)){
        bottomdistance = bottom.IntersectionDistance(&photon);
    }else{
        bottomdistance = INFINITY;
    }
    
    if(PhotonInside && start.GetIntersectionTest(&photon)){
        startdistance = start.IntersectionDistance(&photon);
    }else{
        startdistance = INFINITY;
    }
    
    if(PhotonInside && end.GetIntersectionTest(&photon)){
        enddistance = end.IntersectionDistance(&photon);
    }else{
        bottomdistance = INFINITY;
    }
    
}

double
flexi::IntersectionDistance(){
    double result = INFINITY;
    
    NextInterface = 7;
    
    if(insidedistance<result){
        result = insidedistance;
        NextInterface = 1;
    }
    
    if(outsidedistance<result){
        result = outsidedistance;
        
        NextInterface = 2;
    }
    
    if(topdistance<result){
        result = topdistance;
        
        NextInterface = 3;
    }
    
    if(bottomdistance<result){
        result = bottomdistance;
        
        NextInterface = 4;
    }
    
    if(startdistance<result){
        result = startdistance;
        
        NextInterface = 5;
    }
    
    if(enddistance<result){
        result = enddistance;
        
        NextInterface = 6;
    }

    return result;
}

int&
flexi::GetNextInterface(){
    return NextInterface;
}

Vector3D
flexi::CurvedNormal(Photon& photon, ellipse& ell){
    Point3D intersection = photon.GetPosition() + photon.GetMomentum() * IntersectionDistance();
    Vector3D normal;
    
    normal = (intersection-ell.GetCentre()).Normalise(); //Points out from centre
    return normal;
}

Vector3D
flexi::GetNextNormal(Photon& photon){
    if(NextInterface == 1){
        if(PhotonInside) return (-CurvedNormal(photon, inside));
        else return CurvedNormal(photon,inside);
    }
    
    if(NextInterface == 2){
        if(PhotonInside) return (CurvedNormal(photon,outside));
        else return -CurvedNormal(photon,outside);
    }
    
    if(NextInterface == 3){
        if(PhotonInside) return (-top.GetNormal());
        else return top.GetNormal();
    }
    
    if(NextInterface == 4){
        if(PhotonInside) return (-bottom.GetNormal());
        else return bottom.GetNormal();
    }
    
    if(NextInterface == 5){
        if(PhotonInside) return (-start.GetNormal());
        else return start.GetNormal();
    }
    
    if(NextInterface == 6){
        if(PhotonInside) return (-start.GetNormal());
        else return start.GetNormal();
    }
    
    return Vector3D(0,0,0);
}