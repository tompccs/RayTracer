#include "Box.h"

Box::Box():
base(),top(),left(),right(),front(),back(),height(),refractiveindex()
{}

Box::Box(Sheet* Base, const double& Height):
base(*Base),top(),left(),right(),front(),back(),height(Height),refractiveindex()
{}

Box::Box(const Box& box):
base(box.base),top(box.top),left(box.left),right(box.right),front(box.front),back(box.back),height(box.height),refractiveindex(box.refractiveindex)
{}

Box::~Box()
{}

Box&
Box::Set(Sheet *sheet, const double &h){
    base = *sheet;
    height = h;
    SetSides();
    return (*this);
}

double //Returns volume of box
Box::GetVolume(){
    return GetBase().GetArea()*height;
}

double& //Returns Refractive Index of Box
Box::GetRefractiveIndex(){
    return refractiveindex;
}

void //Sets Refractive Index of Box
Box::SetRefractiveIndex(const double &n){
    refractiveindex = n;
}

Box& //Constructs box
Box::SetSides(){
    Point3D A,B,C,D,E,F,G,H;
    A = base.GetA();
    B = base.GetB();
    C = base.GetC();
    D = base.GetD();
    E = A + base.GetNormal()*height;
    F = C + base.GetNormal()*height;
    H = B + base.GetNormal()*height;
    G = D + base.GetNormal()*height;
    top = Sheet(F,G,E);
    left = Sheet(E,A,F);
    right = Sheet(B,H,D);
    back = Sheet(D,G,C);
    front = Sheet(A,E,B);
    return (*this);
}

Sheet&
Box::GetBase(){
    return base;
}

Sheet&
Box::GetTop(){
    return top;
}
Sheet&
Box::GetLeft(){
    return left;
}

Sheet&
Box::GetRight(){
    return right;
}

Sheet&
Box::GetFront(){
    return front;
}

Sheet&
Box::GetBack(){
    return back;
}

bool //Checks if a point is in a box
Box::PointinBox(Photon* photon){
    int intersections = 0;
    if(base.GetIntersectionTest(photon)){intersections++;}
    if(top.GetIntersectionTest(photon)){intersections++;}
    if(left.GetIntersectionTest(photon)){intersections++;}
    if(right.GetIntersectionTest(photon)){intersections++;}
    if(front.GetIntersectionTest(photon)){intersections++;}
    if(back.GetIntersectionTest(photon)){intersections++;}
    if(intersections==1) return 1;
    return 0;
}

double //Distance to exit
Box::GetInterfaceDistance(Photon* photon){
    vector<double> distances;
    if(base.GetIntersectionTest(photon)) distances.push_back(base.IntersectionDistance(photon));
    if(top.GetIntersectionTest(photon)) distances.push_back(top.IntersectionDistance(photon));
    if(left.GetIntersectionTest(photon)) distances.push_back(left.IntersectionDistance(photon));
    if(right.GetIntersectionTest(photon)) distances.push_back(right.IntersectionDistance(photon));
    if(front.GetIntersectionTest(photon)) distances.push_back(front.IntersectionDistance(photon));
    if(back.GetIntersectionTest(photon)) distances.push_back(back.IntersectionDistance(photon));
    return calc->GetVectorMin(distances);
}

Point3D //Point of interface
Box::GetInterfacePoint(Photon *photon){
    return photon->GetPosition() + photon->GetMomentum()*GetInterfaceDistance(photon);
}

Sheet& //Returns interface sheet
Box::GetInterfaceSheet(Photon *photon){
    if(base.IntersectionDistance(photon)==GetInterfaceDistance(photon)) return base;
    if(top.IntersectionDistance(photon)==GetInterfaceDistance(photon)) return top;
    if(left.IntersectionDistance(photon)==GetInterfaceDistance(photon)) return left;
    if(right.IntersectionDistance(photon)==GetInterfaceDistance(photon)) return right;
    if(front.IntersectionDistance(photon)==GetInterfaceDistance(photon)) return front;
    return back;
}